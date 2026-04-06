#!/bin/bash

# Скрипт для клонирования дисков с GUI

# Функция для логирования
log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" | tee -a /tmp/disk_clone.log >&2
}

# Функция для показа ошибок
show_error() {
    local message="$1"
    local details="$2"
    if [ -n "$details" ]; then
        zenity --error --text="$message\n\nДетали:\n$details" --width=600
    else
        zenity --error --text="$message" --width=600
    fi
}

# Функция для выполнения команд с sudo
run_sudo() {
    if [ "$EUID" -eq 0 ]; then
        # Уже root, выполняем напрямую
        "$@"
    else
        # Используем сохраненный пароль
        if [ -n "$SUDO_PASSWORD" ]; then
            echo "$SUDO_PASSWORD" | sudo -S "$@"
        else
            sudo "$@"
        fi
    fi
}

# Функция очистки при выходе
cleanup() {
    if [ -n "$SUDO_KEEPALIVE_PID" ]; then
        kill $SUDO_KEEPALIVE_PID 2>/dev/null || true
    fi
    unset SUDO_PASSWORD
}

# Устанавливаем trap для очистки
trap cleanup EXIT

# Проверка зависимостей
check_dependencies() {
    local missing_deps=()
    local required_deps=("zenity" "lsblk" "dd" "pv" "gdisk" "partprobe" "growpart" "e2fsck" "resize2fs" "blkid")
    
    for dep in "${required_deps[@]}"; do
        if ! command -v "$dep" >/dev/null 2>&1; then
            missing_deps+=("$dep")
        fi
    done
    
    if [ ${#missing_deps[@]} -gt 0 ]; then
        show_error "Отсутствуют необходимые программы: ${missing_deps[*]}" \
                   "Установите их командой: sudo apt install zenity pv gdisk cloud-guest-utils e2fsprogs util-linux"
        exit 1
    fi
}

# Проверка, что запускается с правильного устройства
check_root_device() {
    local root_device=$(df -h | grep ' /$' | awk '{print $1}')
    if [[ ! "$root_device" =~ ^/dev/mmcblk1 ]]; then
        show_error "Скрипт должен запускаться с устройства mmcblk1" \
                   "Текущее root-устройство: $root_device"
        exit 1
    fi
    log "Root device check passed: $root_device"
}

# Функция для получения прав root (исправленная версия)
get_root_privileges() {
    if [ "$EUID" -ne 0 ]; then
        log "Script not running as root, requesting privileges"
        
        # Запрашиваем пароль через zenity
        local password
        password=$(zenity --entry --hide-text \
            --text="Введите пароль для административных действий" \
            --title="Требуется пароль" --width=400 2>/dev/null)
        
        local zenity_result=$?
        if [ $zenity_result -ne 0 ]; then
            log "Password dialog cancelled by user"
            zenity --info --text="Операция отменена." --width=400
            exit 0
        fi

        if [ -z "$password" ]; then
            log "Empty password provided"
            show_error "Пароль не может быть пустым"
            exit 1
        fi

        # Проверяем введённый пароль (используем обычный sudo)
        log "Verifying password"
        echo "$password" | sudo -S -k true >/dev/null 2>&1
        local sudo_result=$?
        
        if [ $sudo_result -ne 0 ]; then
            log "Password verification failed"
            show_error "Неверный пароль"
            exit 1
        fi

        log "Password verified, setting up sudo session"
        
        # Сохраняем пароль для использования в функциях
        export SUDO_PASSWORD="$password"
        
        # Устанавливаем длительную sudo сессию
        echo "$password" | sudo -S -v
        
        # Запускаем фоновый процесс для поддержания sudo сессии
        (
            while true; do
                sleep 240  # каждые 4 минуты
                echo "$SUDO_PASSWORD" | sudo -S -v >/dev/null 2>&1 || break
            done
        ) &
        export SUDO_KEEPALIVE_PID=$!
        
        log "Sudo session established"
    else
        log "Script already running as root"
    fi
}

# Получение списка дисков с дополнительной информацией
get_disk_list() {
    local exclude_device="$1"
    lsblk -d -n -o NAME,SIZE,MODEL 2>/dev/null | grep -v '^loop' | while read name size model; do
        if [ "$exclude_device" != "/dev/$name" ]; then
            echo "$name ($size) - $model"
        fi
    done
}

# Выбор исходного устройства (исправленная версия)
select_source_device() {
    log "Selecting source device" >&2
    local available_disks
    available_disks=$(get_disk_list "" 2>/dev/null)
    
    if [ -z "$available_disks" ]; then
        show_error "Не найдены доступные диски" >&2
        exit 1
    fi
    
    log "Available source devices:" >&2
    log "$available_disks" >&2
    
    local user_selection
    user_selection=$(echo "$available_disks" | zenity --list --title="Выбор исходного диска" \
        --text="Выберите диск, с которого будет копироваться информация:" \
        --column="Устройство (Размер) - Модель" --width=800 --height=450 2>/dev/null)

    local zenity_result=$?
    if [ $zenity_result -ne 0 ] || [ -z "$user_selection" ]; then
        log "Source device selection cancelled" >&2
        zenity --info --text="Выбор исходного диска отменён." --width=400 >&2
        exit 0
    fi

    log "User selected: $user_selection" >&2
    
    local extracted_device_name
    extracted_device_name=$(echo "$user_selection" | awk '{print $1}' 2>/dev/null)
    local full_device_path="/dev/$extracted_device_name"
    
    log "Extracted device name: $extracted_device_name" >&2
    log "Device path: $full_device_path" >&2
    
    log "Checking if device exists..." >&2
    if [ -e "$full_device_path" ]; then
        log "Device file exists: $full_device_path" >&2
    else
        log "Device file does not exist: $full_device_path" >&2
    fi
    
    if [ -b "$full_device_path" ]; then
        log "Device is block device: $full_device_path" >&2
    else
        log "Device is NOT block device: $full_device_path" >&2
    fi
    
    log "All block devices in /dev:" >&2
    ls -la /dev/ | grep "^b" | grep -E "(mmcblk|nvme|sd)" | head -10 | while read line; do
        log "  $line" >&2
    done
    
    if [ ! -b "$full_device_path" ]; then
        local alternative_paths=("$full_device_path" "/dev/block/$extracted_device_name")
        local found_device_path=""
        
        for alt_path in "${alternative_paths[@]}"; do
            if [ -b "$alt_path" ]; then
                found_device_path="$alt_path"
                log "Found device at alternative path: $alt_path" >&2
                break
            fi
        done
        
        if [ -z "$found_device_path" ]; then
            show_error "Выбранное устройство не найдено: $full_device_path" \
                       "Проверьте список доступных устройств:\n$(lsblk -d -o NAME,SIZE,MODEL 2>/dev/null)" >&2
            exit 1
        fi
        
        full_device_path="$found_device_path"
        log "Using alternative device path: $full_device_path" >&2
    fi
    
    # Выводим только путь к устройству в stdout
    echo "$full_device_path"
}

# Выбор целевого устройства (исправленная версия)
select_target_device() {
    local exclude_source_device="$1"
    log "Selecting target device (excluding $exclude_source_device)" >&2
    
    local available_targets
    available_targets=$(get_disk_list "$exclude_source_device" 2>/dev/null)
    
    if [ -z "$available_targets" ]; then
        show_error "Нет доступных целевых устройств для клонирования" >&2
        exit 1
    fi
    
    log "Available target devices:" >&2
    log "$available_targets" >&2
    
    local target_selection
    target_selection=$(echo "$available_targets" | zenity --list --title="Выбор целевого диска" \
        --text="Выберите диск, на который будет копироваться информация:" \
        --column="Устройство (Размер) - Модель" --width=800 --height=450 2>/dev/null)

    local zenity_result=$?
    if [ $zenity_result -ne 0 ] || [ -z "$target_selection" ]; then
        log "Target device selection cancelled" >&2
        zenity --info --text="Выбор целевого диска отменён." --width=400 >&2
        exit 0
    fi

    log "User selected: $target_selection" >&2
    
    local target_device_name
    target_device_name=$(echo "$target_selection" | awk '{print $1}' 2>/dev/null)
    local target_device_path="/dev/$target_device_name"
    
    log "Extracted device name: $target_device_name" >&2
    log "Device path: $target_device_path" >&2
    
    log "Checking if target device exists..." >&2
    if [ -e "$target_device_path" ]; then
        log "Target device file exists: $target_device_path" >&2
    else
        log "Target device file does not exist: $target_device_path" >&2
    fi
    
    if [ -b "$target_device_path" ]; then
        log "Target device is block device: $target_device_path" >&2
    else
        log "Target device is NOT block device: $target_device_path" >&2
    fi
    
    if [ ! -b "$target_device_path" ]; then
        local target_alt_paths=("$target_device_path" "/dev/block/$target_device_name")
        local found_target_device=""
        
        for target_alt_path in "${target_alt_paths[@]}"; do
            if [ -b "$target_alt_path" ]; then
                found_target_device="$target_alt_path"
                log "Found target device at alternative path: $target_alt_path" >&2
                break
            fi
        done
        
        if [ -z "$found_target_device" ]; then
            show_error "Выбранное устройство не найдено: $target_device_path" \
                       "Проверьте список доступных устройств:\n$(lsblk -d -o NAME,SIZE,MODEL 2>/dev/null)" >&2
            exit 1
        fi
        
        target_device_path="$found_target_device"
        log "Using alternative target device path: $target_device_path" >&2
    fi
    
    # Выводим только путь к устройству в stdout
    echo "$target_device_path"
}

# Подтверждение операции (с улучшенной диагностикой)
confirm_operation() {
    local source_device="$1"
    local target_device="$2"
    
    log "Confirming operation: $source_device -> $target_device"
    
    # Проверяем существование устройств
    if [ ! -b "$source_device" ]; then
        show_error "Исходное устройство не найдено: $source_device"
        exit 1
    fi
    
    if [ ! -b "$target_device" ]; then
        show_error "Целевое устройство не найдено: $target_device"
        exit 1
    fi
    
    # Получаем размеры устройств с отладочной информацией
    local source_size target_size source_size_h target_size_h
    
    log "Getting size for source device: $source_device"
    source_size=$(run_sudo lsblk -b -n -d -o SIZE "$source_device" 2>/dev/null | head -1 | tr -d ' ')
    log "Source size raw: '$source_size'"
    
    log "Getting size for target device: $target_device"
    target_size=$(run_sudo lsblk -b -n -d -o SIZE "$target_device" 2>/dev/null | head -1 | tr -d ' ')
    log "Target size raw: '$target_size'"
    
    if [ -z "$source_size" ] || [ -z "$target_size" ]; then
        log "Failed to get device sizes, trying alternative method"
        
        # Альтернативный метод получения размера
        source_size=$(run_sudo blockdev --getsize64 "$source_device" 2>/dev/null)
        target_size=$(run_sudo blockdev --getsize64 "$target_device" 2>/dev/null)
        
        log "Alternative method - Source size: '$source_size'"
        log "Alternative method - Target size: '$target_size'"
        
        if [ -z "$source_size" ] || [ -z "$target_size" ]; then
            show_error "Не удалось получить размеры устройств" \
                       "Исходное: $source_device\nЦелевое: $target_device\n\nПроверьте права доступа и существование устройств"
            exit 1
        fi
    fi
    
    # Проверяем, что размеры являются числами
    if ! [[ "$source_size" =~ ^[0-9]+$ ]] || ! [[ "$target_size" =~ ^[0-9]+$ ]]; then
        show_error "Получены некорректные размеры устройств" \
                   "Исходное: '$source_size'\nЦелевое: '$target_size'"
        exit 1
    fi
    
    source_size_h=$(numfmt --to=iec-i --suffix=B "$source_size" 2>/dev/null || echo "$source_size bytes")
    target_size_h=$(numfmt --to=iec-i --suffix=B "$target_size" 2>/dev/null || echo "$target_size bytes")
    
    log "Size comparison - Source: $source_size ($source_size_h), Target: $target_size ($target_size_h)"
    
    # Проверяем, что целевое устройство не меньше исходного
    if [ "$target_size" -lt "$source_size" ]; then
        show_error "Целевое устройство слишком маленькое" \
                   "Исходный: $source_size_h\nЦелевой: $target_size_h"
        exit 1
    fi
    
    zenity --question --width=600 \
        --text="Подтвердите операцию клонирования:\n\nИсходный диск: $source_device ($source_size_h)\nЦелевой диск: $target_device ($target_size_h)\n\n⚠️ ВНИМАНИЕ: Все данные на целевом диске будут уничтожены!\n\nПродолжить?" \
        --ok-label="Да, клонировать" \
        --cancel-label="Отмена" 2>/dev/null
    
    local result=$?
    if [ $result -ne 0 ]; then
        log "Operation cancelled by user"
        zenity --info --text="Операция отменена." --width=400
        exit 0
    fi
    
    log "Operation confirmed by user"
}

# Функция клонирования с прогрессом
clone_disk() {
    local source_device="$1"
    local target_device="$2"
    log "Starting disk cloning from $source_device to $target_device"
    local source_size
    source_size=$(run_sudo blockdev --getsize64 "$source_device" 2>/dev/null)
    (
        # Выполняем клонирование с выводом ошибок
        dd if="$source_device" bs=4M status=none | pv -n -s "${source_size}" | dd of="$target_device" bs=4M status=none
    ) 2> >(while read percentage; do
        echo "# Клонирование данных... $percentage%"
    done) | zenity --progress \
        --title="Клонирование диска" \
        --text="Копирование данных..." \
        --pulsate\
        --auto-close \
        --width=600
}

# Проверка разделов после клонирования
verify_partitions() {
    local target_device="$1"
    
    log "Verifying partitions on $target_device"
    
    # Обновляем таблицу разделов
    run_sudo partprobe "$target_device" >/dev/null 2>&1 || true
    sleep 3
    
    # Проверяем наличие разделов
    local partition_count
    partition_count=$(lsblk -r -n -o NAME "$target_device" 2>/dev/null | grep -c "${target_device##*/}p" || echo "0")
    
    if [ "$partition_count" -eq 0 ]; then
        show_error "На целевом диске не найдены разделы"
        exit 1
    fi
    
    log "Found $partition_count partitions on target device"
}

# Расширение диска и файловой системы
expand_disk() {
    local target_device="$1"
    local target_partition="${target_device}p1"
    
    log "Starting disk expansion for $target_device"
    
    local expand_result=0
    (
        echo "10"
        echo "# Исправление таблицы GPT..."
        
        # Исправляем GPT таблицу
        echo -e "x\ne\nw\ny" | run_sudo gdisk "$target_device" >/dev/null 2>&1 || true
        sleep 2

        echo "25"
        echo "# Обновление таблицы разделов..."
        
        run_sudo partprobe "$target_device" >/dev/null 2>&1 || true
        sleep 3

        echo "40"
        echo "# Расширение раздела..."
        
        # Пытаемся расширить раздел
        if ! run_sudo growpart "$target_device" 1 >/dev/null 2>&1; then
            # Альтернативный метод через parted
            echo -e "resizepart\n1\nYes\n100%" | run_sudo parted "$target_device" ---pretend-input-tty >/dev/null 2>&1 || true
        fi
        sleep 2

        echo "55"
        echo "# Размонтирование раздела..."
        
        run_sudo umount "$target_partition" >/dev/null 2>&1 || true
        sleep 1

        echo "70"
        echo "# Проверка файловой системы..."
        
        run_sudo e2fsck -f -y "$target_partition" >/dev/null 2>&1 || {
            log "e2fsck returned non-zero, but continuing"
        }
        sleep 2

        echo "85"
        echo "# Расширение файловой системы..."
        
        run_sudo resize2fs "$target_partition" >/dev/null 2>&1 || {
            log "resize2fs had issues, but continuing"
        }
        sleep 2

        echo "100"
        echo "# Расширение завершено"
        
    ) | zenity --progress --title="Расширение диска" \
        --text="Расширение раздела и файловой системы..." \
        --percentage=0 --auto-close --width=600 2>/dev/null
    
    expand_result=${PIPESTATUS[0]}
    
    # Не считаем это критической ошибкой, так как основное клонирование уже выполнено
    if [ $expand_result -ne 0 ]; then
        log "Warning: Disk expansion had some issues, but continuing"
    else
        log "Disk expansion completed successfully"
    fi
}

# Обновление системных конфигураций
update_system_configs() {
    local source_device="$1"
    local target_device="$2"
    
    log "Updating system configuration files"
    
    # Создаем временные точки монтирования
    local temp_mount boot_mount
    temp_mount=$(mktemp -d)
    boot_mount=$(mktemp -d)
    
    # Монтируем целевой раздел
    if ! run_sudo mount "${target_device}p1" "$temp_mount" 2>/dev/null; then
        log "Warning: Could not mount target partition for config update"
        rmdir "$temp_mount" "$boot_mount" 2>/dev/null || true
        return 0
    fi
    
    # Получаем UUID разделов
    local new_uuid old_uuid
    new_uuid=$(blkid -s UUID -o value "${target_device}p1" 2>/dev/null)
    old_uuid=$(blkid -s UUID -o value "${source_device}p1" 2>/dev/null)
    
    if [ -z "$new_uuid" ] || [ -z "$old_uuid" ]; then
        log "Warning: Could not get UUIDs, skipping configuration update"
        run_sudo umount "$temp_mount" 2>/dev/null || true
        rmdir "$temp_mount" "$boot_mount" 2>/dev/null || true
        return 0
    fi
    
    log "UUID update: $old_uuid -> $new_uuid"
    
    # Обновляем fstab в целевой системе
    if [ -f "$temp_mount/etc/fstab" ]; then
        run_sudo sed -i "s|UUID=$old_uuid|UUID=$new_uuid|g" "$temp_mount/etc/fstab" 2>/dev/null || true
        log "Updated /etc/fstab"
    else
        log "Warning: /etc/fstab not found"
    fi
    
    # Обновляем armbianEnv.txt
    local armbian_env_paths=(
        "$temp_mount/boot/armbianEnv.txt"
        "$temp_mount/boot/orangepiEnv.txt"
    )
    
    # Проверяем, есть ли отдельный boot раздел
    if [ -b "${target_device}p2" ]; then
        log "Detected separate boot partition, mounting ${target_device}p2"
        if run_sudo mount "${target_device}p2" "$boot_mount" 2>/dev/null; then
            armbian_env_paths+=(
                "$boot_mount/armbianEnv.txt"
                "$boot_mount/orangepiEnv.txt"
            )
        else
            log "Warning: Could not mount boot partition ${target_device}p2"
        fi
    fi
    
    # Обновляем все найденные конфигурационные файлы
    for env_file in "${armbian_env_paths[@]}"; do
        if [ -f "$env_file" ]; then
            log "Processing $env_file"
            
            # Обновляем различные форматы UUID записей
            run_sudo sed -i "s|rootdev=UUID=$old_uuid|rootdev=UUID=$new_uuid|g" "$env_file" 2>/dev/null || true
            run_sudo sed -i "s|root=UUID=$old_uuid|root=UUID=$new_uuid|g" "$env_file" 2>/dev/null || true
            run_sudo sed -i "s|UUID=$old_uuid|UUID=$new_uuid|g" "$env_file" 2>/dev/null || true
            
            # Также обновляем записи с PARTUUID если они есть
            local old_partuuid new_partuuid
            old_partuuid=$(blkid -s PARTUUID -o value "${source_device}p1" 2>/dev/null)
            new_partuuid=$(blkid -s PARTUUID -o value "${target_device}p1" 2>/dev/null)
            
            if [ -n "$old_partuuid" ] && [ -n "$new_partuuid" ]; then
                run_sudo sed -i "s|PARTUUID=$old_partuuid|PARTUUID=$new_partuuid|g" "$env_file" 2>/dev/null || true
                run_sudo sed -i "s|rootdev=PARTUUID=$old_partuuid|rootdev=PARTUUID=$new_partuuid|g" "$env_file" 2>/dev/null || true
                run_sudo sed -i "s|root=PARTUUID=$old_partuuid|root=PARTUUID=$new_partuuid|g" "$env_file" 2>/dev/null || true
                log "Updated PARTUUID in $(basename "$env_file"): $old_partuuid -> $new_partuuid"
            fi
            
            log "Updated $(basename "$env_file"): UUID $old_uuid -> $new_uuid"
        fi
    done
    
    # Обновляем /etc/kernel/cmdline если существует
    if [ -f "$temp_mount/etc/kernel/cmdline" ]; then
        run_sudo sed -i "s|UUID=$old_uuid|UUID=$new_uuid|g" "$temp_mount/etc/kernel/cmdline" 2>/dev/null || true
        if [ -n "$old_partuuid" ] && [ -n "$new_partuuid" ]; then
            run_sudo sed -i "s|PARTUUID=$old_partuuid|PARTUUID=$new_partuuid|g" "$temp_mount/etc/kernel/cmdline" 2>/dev/null || true
        fi
        log "Updated /etc/kernel/cmdline"
    fi
    
    # Размонтируем
    run_sudo umount "$boot_mount" 2>/dev/null || true
    run_sudo umount "$temp_mount" 2>/dev/null || true
    rmdir "$boot_mount" "$temp_mount" 2>/dev/null || true
    
    log "System configuration update completed"
}

# Основная функция
main() {
    log "Starting disk cloning script v2.2"
    
    check_dependencies
    check_root_device
    get_root_privileges
    
    local selected_source_device selected_target_device
    selected_source_device=$(select_source_device)
    log "Selected source device: $selected_source_device"
    
    selected_target_device=$(select_target_device "$selected_source_device")
    log "Selected target device: $selected_target_device"
    
    confirm_operation "$selected_source_device" "$selected_target_device"
    
    log "Before cloning"
    clone_disk "$selected_source_device" "$selected_target_device"
    log "After cloning"
    
    # Проверка наличия разделов
    local partition_count
    partition_count=$(lsblk -r -n -o NAME "$selected_target_device" 2>/dev/null | grep -c "${selected_target_device##*/}p" || echo "0")
    if [ "$partition_count" -eq 0 ]; then
        show_error "Клонирование не выполнено: на целевом диске не найдены разделы"
        exit 1
    fi
    log "Found $partition_count partitions on target device after cloning"
    
    verify_partitions "$selected_target_device"
    expand_disk "$selected_target_device"
    update_system_configs "$selected_source_device" "$selected_target_device"
    
    log "All operations completed successfully"
    zenity --info --text="Диск успешно склонирован и расширен!\n\n" --width=500 2>/dev/null
    
    zenity --question --text="Операция завершена успешно.\n\nВыключить компьютер?" \
        --ok-label="Выключить" --cancel-label="Продолжить работу" --width=400 2>/dev/null
    
    local shutdown_choice=$?
    if [ $shutdown_choice -eq 0 ]; then
        log "User requested shutdown"
        run_sudo poweroff
    else
        log "User chose to continue working"
    fi
}

# Запуск основной функции
main "$@"
