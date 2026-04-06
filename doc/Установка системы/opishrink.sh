#!/usr/bin/env bash

# Project: ArmbianShrink (based on PiShrink)
# Description: ArmbianShrink is a bash script that automatically shrinks an Armbian image for Orange Pi 5 Plus
# Original PiShrink: https://github.com/Drewsif/PiShrink
# Modified for: Armbian Orange Pi 5 Plus Ubuntu 24.04

version="v1.0.2-armbian"

CURRENT_DIR="$(pwd)"
SCRIPTNAME="${0##*/}"
MYNAME="${SCRIPTNAME%.*}"
LOGFILE="${CURRENT_DIR}/${SCRIPTNAME%.*}.log"
REQUIRED_TOOLS="parted losetup tune2fs md5sum e2fsck resize2fs partprobe"
ZIPTOOLS=("gzip xz")
declare -A ZIP_PARALLEL_TOOL=( [gzip]="pigz" [xz]="xz" )
declare -A ZIP_PARALLEL_OPTIONS=( [gzip]="-f9" [xz]="-T0" )
declare -A ZIPEXTENSIONS=( [gzip]="gz" [xz]="xz" )

# Progress tracking
TOTAL_STEPS=10
CURRENT_STEP=0

function show_progress() {
    CURRENT_STEP=$((CURRENT_STEP + 1))
    local percent=$((CURRENT_STEP * 100 / TOTAL_STEPS))
    printf "\r[%3d%%] Step %d/%d: %s" "$percent" "$CURRENT_STEP" "$TOTAL_STEPS" "$1"
    echo ""
}

function info() {
    echo "$SCRIPTNAME: $1"
}

function error() {
    echo -n "$SCRIPTNAME: ERROR occurred in line $1: "
    shift
    echo "$@"
}

function cleanup() {
    if losetup "$loopback" &>/dev/null; then
        losetup -d "$loopback"
    fi
    if [ "$debug" = true ]; then
        local old_owner=$(stat -c %u:%g "$src")
        chown "$old_owner" "$LOGFILE"
    fi
}

function logVariables() {
    if [ "$debug" = true ]; then
        echo "Line $1" >> "$LOGFILE"
        shift
        local v var
        for var in "$@"; do
            eval "v=\$$var"
            echo "$var: $v" >> "$LOGFILE"
        done
    fi
}

function checkFilesystem() {
    show_progress "Checking filesystem integrity"
    e2fsck -pf "$loopback"
    (( $? < 4 )) && return

    info "Filesystem error detected!"

    info "Trying to recover corrupted filesystem"
    e2fsck -y "$loopback"
    (( $? < 4 )) && return

    if [[ $repair == true ]]; then
        info "Trying to recover corrupted filesystem - Phase 2"
        e2fsck -fy -b 32768 "$loopback"
        (( $? < 4 )) && return
    fi
    error $LINENO "Filesystem recoveries failed. Giving up..."
    exit 9
}

function set_armbian_autoexpand() {
    show_progress "Setting up Armbian auto-expansion"

    # Создаем временную директорию для монтирования
    mountdir=$(mktemp -d)
    partprobe "$loopback"
    sleep 3
    umount "$loopback" > /dev/null 2>&1
    mount "$loopback" "$mountdir" -o rw
    if (( $? != 0 )); then
        info "Unable to mount loopback, autoexpand will not be enabled"
        return
    fi

    if [ ! -d "$mountdir/etc" ]; then
        info "/etc not found, autoexpand will not be enabled"
        umount "$mountdir"
        return
    fi

    # Создаем флаг для расширения
    touch "$mountdir/etc/.resize_flag"
    info "Created resize flag at /etc/.resize_flag"

    # Создаем директорию если её нет
    mkdir -p "$mountdir/usr/local/bin"

    # Создаем улучшенный скрипт для расширения файловой системы
    cat <<'EOF' > "$mountdir/usr/local/bin/resize-rootfs.sh"
#!/bin/bash

# Resize script for Armbian auto-expansion
# Version: 2.0 - Improved reliability

set -e

FLAG_FILE="/etc/.resize_flag"
LOG_FILE="/var/log/resize-rootfs.log"

# Функция логирования
log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" | tee -a "$LOG_FILE"
}

# Проверяем наличие флага
if [ ! -f "$FLAG_FILE" ]; then
    log "No resize flag found, exiting"
    exit 0
fi

log "Starting root filesystem resize process"

# Определение root-устройства
ROOT_DEV=""

# Метод 1: через /proc/cmdline
if [ -z "$ROOT_DEV" ]; then
    CMDLINE_ROOT=$(grep -o 'root=[^ ]*' /proc/cmdline 2>/dev/null | cut -d'=' -f2 | head -1)
    if [ -n "$CMDLINE_ROOT" ] && [ -b "$CMDLINE_ROOT" ]; then
        ROOT_DEV="$CMDLINE_ROOT"
        log "Root device detected from /proc/cmdline: $ROOT_DEV"
    fi
fi

# Метод 2: через findmnt
if [ -z "$ROOT_DEV" ]; then
    FINDMNT_ROOT=$(findmnt -n -o SOURCE / 2>/dev/null | head -1)
    if [ -n "$FINDMNT_ROOT" ] && [ -b "$FINDMNT_ROOT" ]; then
        ROOT_DEV="$FINDMNT_ROOT"
        log "Root device detected from findmnt: $ROOT_DEV"
    fi
fi

# Метод 3: через /proc/mounts
if [ -z "$ROOT_DEV" ]; then
    MOUNTS_ROOT=$(grep " / " /proc/mounts | cut -d' ' -f1 | head -1)
    if [ -n "$MOUNTS_ROOT" ] && [ -b "$MOUNTS_ROOT" ]; then
        ROOT_DEV="$MOUNTS_ROOT"
        log "Root device detected from /proc/mounts: $ROOT_DEV"
    fi
fi

if [ -z "$ROOT_DEV" ]; then
    log "ERROR: Could not detect root device"
    exit 1
fi

log "Using root device: $ROOT_DEV"

# Извлечение устройства и номера раздела
DEVICE=""
PART_NUM=""

# Поддержка различных типов устройств
if [[ $ROOT_DEV =~ ^/dev/(mmcblk[0-9]+)p([0-9]+)$ ]]; then
    DEVICE="/dev/${BASH_REMATCH[1]}"
    PART_NUM=${BASH_REMATCH[2]}
    log "Detected MMC device: $DEVICE, partition: $PART_NUM"
elif [[ $ROOT_DEV =~ ^/dev/(nvme[0-9]+n[0-9]+)p([0-9]+)$ ]]; then
    DEVICE="/dev/${BASH_REMATCH[1]}"
    PART_NUM=${BASH_REMATCH[2]}
    log "Detected NVMe device: $DEVICE, partition: $PART_NUM"
elif [[ $ROOT_DEV =~ ^/dev/(sd[a-z]+)([0-9]+)$ ]]; then
    DEVICE="/dev/${BASH_REMATCH[1]}"
    PART_NUM=${BASH_REMATCH[2]}
    log "Detected SATA/USB device: $DEVICE, partition: $PART_NUM"
else
    log "ERROR: Unsupported device type: $ROOT_DEV"
    exit 1
fi

# Проверка существования устройства
if [ ! -b "$DEVICE" ]; then
    log "ERROR: Device $DEVICE does not exist"
    exit 1
fi

# Получение текущего размера диска
DISK_SIZE=$(blockdev --getsize64 "$DEVICE" 2>/dev/null)
if [ -z "$DISK_SIZE" ] || [ "$DISK_SIZE" -eq 0 ]; then
    log "ERROR: Could not get disk size for $DEVICE"
    exit 1
fi

log "Disk size: $DISK_SIZE bytes"

# Получение информации о разделе
PART_INFO=$(parted -s "$DEVICE" unit B print 2>/dev/null | grep "^ *$PART_NUM ")
if [ -z "$PART_INFO" ]; then
    log "ERROR: Could not get partition $PART_NUM information"
    exit 1
fi

PART_START=$(echo "$PART_INFO" | awk '{print $2}' | tr -d 'B')
PART_END=$(echo "$PART_INFO" | awk '{print $3}' | tr -d 'B')

log "Current partition: start=$PART_START, end=$PART_END"

# Вычисление максимального размера
# Оставляем небольшой буфер в конце диска (1MB)
MAX_END=$((DISK_SIZE - 1048576))

if [ "$PART_END" -ge "$MAX_END" ]; then
    log "Partition is already at maximum size, no resize needed"
    rm -f "$FLAG_FILE"
    exit 0
fi

log "Expanding partition $PART_NUM on device $DEVICE to maximum size"

# Расширение раздела с использованием parted
{
    echo "resizepart"
    echo "$PART_NUM"
    echo "Yes"
    echo "${MAX_END}B"
    echo "quit"
} | parted "$DEVICE" ---pretend-input-tty 2>&1 | tee -a "$LOG_FILE"

if [ ${PIPESTATUS[1]} -ne 0 ]; then
    log "ERROR: Failed to resize partition with parted"
    exit 1
fi

log "Partition resized successfully"

# Обновление таблицы разделов
log "Updating partition table"
partprobe "$DEVICE" 2>&1 | tee -a "$LOG_FILE"
sleep 2

# Дополнительное обновление ядра о изменениях
if command -v blockdev >/dev/null 2>&1; then
    blockdev --rereadpt "$DEVICE" 2>&1 | tee -a "$LOG_FILE"
fi

sleep 2

# Проверка, что раздел действительно изменился
NEW_PART_SIZE=$(blockdev --getsize64 "$ROOT_DEV" 2>/dev/null)
if [ -n "$NEW_PART_SIZE" ]; then
    log "New partition size: $NEW_PART_SIZE bytes"
fi

# Расширение файловой системы
log "Expanding filesystem on $ROOT_DEV"

# Проверяем файловую систему перед расширением
if command -v e2fsck >/dev/null 2>&1; then
    log "Checking filesystem before resize"
    e2fsck -f -y "$ROOT_DEV" 2>&1 | tee -a "$LOG_FILE" || {
        log "WARNING: filesystem check returned non-zero status, continuing anyway"
    }
fi

# Расширяем файловую систему
if ! resize2fs "$ROOT_DEV" 2>&1 | tee -a "$LOG_FILE"; then
    log "ERROR: Failed to resize filesystem"
    exit 1
fi

log "Filesystem resized successfully"

# Проверяем результат
FS_SIZE=$(df -h / | tail -1 | awk '{print $2}')
log "New filesystem size: $FS_SIZE"

# Удаление флага после успешного расширения
rm -f "$FLAG_FILE"
log "Resize flag removed - resize completed successfully"

# Отключаем службу после успешного выполнения
if command -v systemctl >/dev/null 2>&1; then
    systemctl disable resize-rootfs.service 2>/dev/null || true
    log "Resize service disabled"
fi

log "Root filesystem resize process completed successfully"
EOF

    # Делаем скрипт исполняемым
    chmod +x "$mountdir/usr/local/bin/resize-rootfs.sh"

    # Создаем улучшенную systemd-службу
    cat <<'EOF' > "$mountdir/etc/systemd/system/resize-rootfs.service"
[Unit]
Description=Resize root filesystem to fill partition
Documentation=man:resize2fs(8)
DefaultDependencies=false
Conflicts=shutdown.target
After=systemd-remount-fs.service
Before=local-fs-pre.target shutdown.target
Wants=local-fs-pre.target

[Service]
Type=oneshot
RemainAfterExit=yes
ExecStart=/usr/local/bin/resize-rootfs.sh
StandardOutput=journal+console
StandardError=journal+console
TimeoutSec=300

[Install]
WantedBy=local-fs.target
EOF

    # Создаем директорию для симлинков systemd
    mkdir -p "$mountdir/etc/systemd/system/local-fs.target.wants"
    
    # Активируем службу через симлинк
    ln -sf "/etc/systemd/system/resize-rootfs.service" "$mountdir/etc/systemd/system/local-fs.target.wants/resize-rootfs.service"

    # Дополнительно создаем альтернативный механизм через rc.local для совместимости
    if [ -f "$mountdir/etc/rc.local" ]; then
        # Делаем бэкап оригинального rc.local
        cp "$mountdir/etc/rc.local" "$mountdir/etc/rc.local.backup"
    fi

    # Создаем или модифицируем rc.local
    cat <<'EOF' > "$mountdir/etc/rc.local"
#!/bin/bash
# Auto-resize root filesystem if flag exists
if [ -f /etc/.resize_flag ]; then
    echo "Resize flag detected, starting filesystem expansion..."
    /usr/local/bin/resize-rootfs.sh
fi

# Restore original rc.local if it existed
if [ -f /etc/rc.local.backup ] && [ ! -f /etc/.resize_flag ]; then
    mv /etc/rc.local.backup /etc/rc.local
    exec /etc/rc.local "$@"
fi

exit 0
EOF

    # Делаем rc.local исполняемым
    chmod +x "$mountdir/etc/rc.local"

    # Проверяем и включаем rc-local service если нужно
    if [ -f "$mountdir/lib/systemd/system/rc-local.service" ] || [ -f "$mountdir/etc/systemd/system/rc-local.service" ]; then
        mkdir -p "$mountdir/etc/systemd/system/multi-user.target.wants"
        ln -sf "/lib/systemd/system/rc-local.service" "$mountdir/etc/systemd/system/multi-user.target.wants/rc-local.service" 2>/dev/null || true
    fi

    # Создаем информационный файл для пользователя
    cat <<'EOF' > "$mountdir/etc/armbian-resize-info.txt"
This Armbian image has been prepared with auto-resize functionality.

On first boot, the root filesystem will automatically expand to fill
the entire available disk space.

The resize process will:
1. Detect the root device automatically
2. Expand the root partition to maximum size
3. Expand the filesystem to fill the partition
4. Remove the resize flag and disable the service

Log file: /var/log/resize-rootfs.log
EOF

    # Очищаем и завершаем
    umount "$mountdir"
    rmdir "$mountdir"
    info "Auto-expansion setup completed with improved reliability"
    info "- SystemD service: resize-rootfs.service"
    info "- Fallback mechanism: rc.local"
    info "- Multiple device detection methods"
    info "- Comprehensive logging to /var/log/resize-rootfs.log"
}

help() {
    local help
    read -r -d '' help << EOM
Usage: $0 [-adhnrsvzZ] imagefile.img [newimagefile.img]

ArmbianShrink - Shrink Armbian images for Orange Pi 5 Plus

  -s         Don't expand filesystem when image is booted the first time
  -v         Be verbose
  -n         Disable automatic update checking
  -r         Use advanced filesystem repair option if the normal one fails
  -z         Compress image after shrinking with gzip
  -Z         Compress image after shrinking with xz
  -a         Compress image in parallel using multiple cores
  -d         Write debug messages in a debug log file
EOM
    echo "$help"
    exit 1
}

should_skip_autoexpand=false
debug=false
update_check=true
repair=false
parallel=false
verbose=false
ziptool=""

while getopts ":adnhrsvzZ" opt; do
    case "${opt}" in
        a) parallel=true;;
        d) debug=true;;
        n) update_check=false;;
        h) help;;
        r) repair=true;;
        s) should_skip_autoexpand=true ;;
        v) verbose=true;;
        z) ziptool="gzip";;
        Z) ziptool="xz";;
        *) help;;
    esac
done
shift $((OPTIND-1))

if [ "$debug" = true ]; then
    info "Creating log file $LOGFILE"
    rm "$LOGFILE" &>/dev/null
    exec 1> >(stdbuf -i0 -o0 -e0 tee -a "$LOGFILE" >&1)
    exec 2> >(stdbuf -i0 -o0 -e0 tee -a "$LOGFILE" >&2)
fi

echo -e "ArmbianShrink $version - Orange Pi 5 Plus Image Shrinker\n"

show_progress "Initializing"

# Try and check for updates (disabled by default since this is a custom version)
if $update_check; then
    info "Update checking disabled for custom ArmbianShrink version"
fi

#Args
src="$1"
img="$1"

#Usage checks
if [[ -z "$img" ]]; then
    help
fi

if [[ ! -f "$img" ]]; then
    error $LINENO "$img is not a file..."
    exit 2
fi
if (( EUID != 0 )); then
    error $LINENO "You need to be running as root."
    exit 3
fi

# set locale to POSIX(English) temporarily
export LANGUAGE=POSIX
export LC_ALL=POSIX
export LANG=POSIX

# check selected compression tool is supported and installed
if [[ -n $ziptool ]]; then
    if [[ ! " ${ZIPTOOLS[@]} " =~ $ziptool ]]; then
        error $LINENO "$ziptool is an unsupported ziptool."
        exit 17
    else
        if [[ $parallel == true && $ziptool == "gzip" ]]; then
            REQUIRED_TOOLS="$REQUIRED_TOOLS pigz"
        else
            REQUIRED_TOOLS="$REQUIRED_TOOLS $ziptool"
        fi
    fi
fi

show_progress "Checking required tools"

#Check that what we need is installed
for command in $REQUIRED_TOOLS; do
    command -v $command >/dev/null 2>&1
    if (( $? != 0 )); then
        error $LINENO "$command is not installed."
        exit 4
    fi
done

#Copy to new file if requested
if [ -n "$2" ]; then
    show_progress "Copying image file"
    f="$2"
    if [[ -n $ziptool && "${f##*.}" == "${ZIPEXTENSIONS[$ziptool]}" ]]; then
        f="${f%.*}"
    fi
    info "Copying $1 to $f..."
    cp --reflink=auto --sparse=always "$1" "$f"
    if (( $? != 0 )); then
        error $LINENO "Could not copy file..."
        exit 5
    fi
    old_owner=$(stat -c %u:%g "$1")
    chown "$old_owner" "$f"
    img="$f"
fi

# cleanup at script exit
trap cleanup EXIT

show_progress "Gathering partition information"

#Gather info
info "Gathering data"
beforesize="$(ls -lh "$img" | cut -d ' ' -f 5)"
parted_output="$(parted -ms "$img" unit B print)"
rc=$?
if (( $rc )); then
    error $LINENO "parted failed with rc $rc"
    info "Possibly invalid image. Run 'parted $img unit B print' manually to investigate"
    exit 6
fi
partnum="$(echo "$parted_output" | tail -n 1 | cut -d ':' -f 1)"
partstart="$(echo "$parted_output" | tail -n 1 | cut -d ':' -f 2 | tr -d 'B')"
if [ -z "$(parted -s "$img" unit B print | grep "$partstart" | grep logical)" ]; then
    parttype="primary"
else
    parttype="logical"
fi
loopback="$(losetup -f --show -o "$partstart" "$img")"
tune2fs_output="$(tune2fs -l "$loopback")"
rc=$?
if (( $rc )); then
    echo "$tune2fs_output"
    error $LINENO "tune2fs failed. Unable to shrink this type of image"
    exit 7
fi

currentsize="$(echo "$tune2fs_output" | grep '^Block count:' | tr -d ' ' | cut -d ':' -f 2)"
blocksize="$(echo "$tune2fs_output" | grep '^Block size:' | tr -d ' ' | cut -d ':' -f 2)"

logVariables $LINENO beforesize parted_output partnum partstart parttype tune2fs_output currentsize blocksize

#Check if we should make Armbian expand rootfs on next boot
if [ "$parttype" == "logical" ]; then
    echo "WARNING: ArmbianShrink does not yet support autoexpanding of logical partitions"
elif [ "$should_skip_autoexpand" = false ]; then
    set_armbian_autoexpand
else
    echo "Skipping autoexpanding process..."
    CURRENT_STEP=$((CURRENT_STEP + 1))
fi

#Make sure filesystem is ok
checkFilesystem

show_progress "Calculating minimum filesystem size"

if ! minsize=$(resize2fs -P "$loopback"); then
    rc=$?
    error $LINENO "resize2fs failed with rc $rc"
    exit 10
fi
minsize=$(cut -d ':' -f 2 <<< "$minsize" | tr -d ' ')
logVariables $LINENO currentsize minsize

if [[ $currentsize -eq $minsize ]]; then
    info "Filesystem already shrunk to smallest size. Skipping filesystem shrinking"
    # Skip several steps if no shrinking needed
    CURRENT_STEP=$((CURRENT_STEP + 4))
else
    show_progress "Calculating optimal size with buffer space"
    
    #Add some free space to the end of the filesystem
    extra_space=$(($currentsize - $minsize))
    logVariables $LINENO extra_space
    for space in 5000 1000 100; do
        if [[ $extra_space -gt $space ]]; then
            minsize=$(($minsize + $space))
            break
        fi
    done

    logVariables $LINENO minsize

    show_progress "Shrinking filesystem"
    
    #Shrink filesystem
    info "Shrinking filesystem from $currentsize blocks to $minsize blocks"
    if [ -z "$mountdir" ]; then
        mountdir=$(mktemp -d)
    fi

    resize2fs -p "$loopback" $minsize
    rc=$?
    if (( $rc )); then
        error $LINENO "resize2fs failed with rc $rc"
        mount "$loopback" "$mountdir"
        if [ -f "$mountdir/etc/rc.local.bak" ]; then
            mv "$mountdir/etc/rc.local.bak" "$mountdir/etc/rc.local"
        fi
        umount "$mountdir"
        losetup -d "$loopback"
        exit 12
    else
        show_progress "Zeroing free space for better compression"
        
        info "Zeroing any free space left"
        mount "$loopback" "$mountdir"
        dd if=/dev/zero of="$mountdir/ArmbianShrink_zero_file" bs=1M 2>/dev/null || true
        info "Zeroed $(ls -lh "$mountdir/ArmbianShrink_zero_file" 2>/dev/null | cut -d ' ' -f 5 || echo "some space")"
        rm -f "$mountdir/ArmbianShrink_zero_file"
        umount "$mountdir"
    fi
    sleep 1

    show_progress "Shrinking partition"
    
    #Shrink partition
    info "Shrinking partition"
    partnewsize=$(($minsize * $blocksize))
    newpartend=$(($partstart + $partnewsize))
    logVariables $LINENO partnewsize newpartend
    parted -s -a minimal "$img" rm "$partnum"
    rc=$?
    if (( $rc )); then
        error $LINENO "parted failed with rc $rc"
        exit 13
    fi

    parted -s "$img" unit B mkpart "$parttype" "$partstart" "$newpartend"
    rc=$?
    if (( $rc )); then
        error $LINENO "parted failed with rc $rc"
        exit 14
    fi

    show_progress "Truncating image file"
    
    #Truncate the file
    info "Truncating image"
    endresult=$(parted -ms "$img" unit B print free)
    rc=$?
    if (( $rc )); then
        error $LINENO "parted failed with rc $rc"
        exit 15
    fi

    endresult=$(tail -1 <<< "$endresult" | cut -d ':' -f 2 | tr -d 'B')
    logVariables $LINENO endresult
    truncate -s "$endresult" "$img"
    rc=$?
    if (( $rc )); then
        error $LINENO "truncate failed with rc $rc"
        exit 16
    fi
fi

# handle compression
if [[ -n $ziptool ]]; then
    show_progress "Compressing image with $ziptool"
    
    options=""
    envVarname="${MYNAME^^}_${ziptool^^}" # ARMBIANSHRINK_GZIP or ARMBIANSHRINK_XZ
    [[ $parallel == true ]] && options="${ZIP_PARALLEL_OPTIONS[$ziptool]}"
    [[ -v $envVarname ]] && options="${!envVarname}"
    [[ $verbose == true ]] && options="$options -v"

    if [[ $parallel == true ]]; then
        parallel_tool="${ZIP_PARALLEL_TOOL[$ziptool]}"
        info "Using $parallel_tool on the shrunk image"
        if ! $parallel_tool ${options} "$img"; then
            rc=$?
            error $LINENO "$parallel_tool failed with rc $rc"
            exit 18
    fi
    else
        info "Using $ziptool on the shrunk image"
        if ! $ziptool ${options} "$img"; then
            rc=$?
            error $LINENO "$ziptool failed with rc $rc"
            exit 19
        fi
    fi
    img=$img.${ZIPEXTENSIONS[$ziptool]}
else
    CURRENT_STEP=$((CURRENT_STEP + 1))
fi

show_progress "Finalizing"

aftersize=$(ls -lh "$img" | cut -d ' ' -f 5)
logVariables $LINENO aftersize

echo ""
info "Successfully shrunk $img from $beforesize to $aftersize"
info "Image is ready for flashing to Orange Pi 5 Plus"