# Настройка SFTP-сервера на ПК с Windows

## Этап 1: Установка OpenSSH Server на Windows через PowerShell

Запустите **PowerShell от имени администратора** и выполните:

```powershell
Add-WindowsCapability -Online -Name OpenSSH.Server~~~~0.0.1.0
```

Проверка успешности установки:

```powershell
Get-WindowsCapability -Online | Where-Object Name -like 'OpenSSH.Server*'
```

Ожидаемый результат: `State : Installed`

---

## Этап 2: Создание пользователя для SFTP

Запустите **PowerShell от имени администратора**:

```powershell
net user sftp_user ВАШ_ПАРОЛЬ /add
```

Замените:

- `sftp_user` — имя пользователя (латинские буквы, без пробелов)
- `ВАШ_ПАРОЛЬ` — надёжный пароль (минимум 8 символов, заглавные + строчные буквы + цифры)

**Пример:**

```powershell
net user linia Linia2026! /add
```

---

## Этап 3: Подготовка каталога обмена

### 3.1 Создание каталога

Создайте папку, в которую будут передаваться файлы. Например:

```powershell
mkdir C:\SFTPData
```

### 3.2 Назначение прав доступа

Дайте SFTP-пользователю полный доступ к каталогу:

```powershell
icacls C:\SFTPData /grant sftp_user:(OI)(CI)F
```

Расшифровка флагов:

- `(OI)` — наследование для файлов
- `(CI)` — наследование для подкаталогов
- `F` — полный доступ (чтение, запись, удаление)

---

## Этап 4: Настройка OpenSSH Server

### 4.1 Редактирование конфигурации

Откройте файл конфигурации в **Блокноте от имени администратора**:

```powershell
notepad C:\ProgramData\ssh\sshd_config
```

Убедитесь, что следующие строки присутствуют и **не закомментированы** (без символа `#` в начале):

```
Port 22
PasswordAuthentication yes
Subsystem sftp sftp-server.exe
```

### 4.2 Ограничение доступа SFTP-пользователя определённым каталогом

Добавьте **в конец** файла `sshd_config`:

```
Match User sftp_user
    ChrootDirectory C:\SFTPData
    ForceCommand internal-sftp
    AllowTcpForwarding no
```

```powershell
icacls C:\SFTPData /setowner "BUILTIN\Administrators"
icacls C:\SFTPData /grant "BUILTIN\Administrators:(OI)(CI)F"
icacls C:\SFTPData /grant "sftp_user:(OI)(CI)M"
icacls C:\SFTPData /remove:g "Authenticated Users"
```

### 4.3 Запуск службы

```powershell
Start-Service sshd
```

### 4.4 Настройка автозапуска

Чтобы SFTP-сервер запускался вместе с Windows:

```powershell
Set-Service -Name sshd -StartupType Automatic
```

---

## Этап 5: Определение IP-адреса SFTP-сервера

На ПК с Windows выполните:

```powershell
ipconfig
```

Найдите строку **IPv4-адрес** в секции активного сетевого адаптера:

```
Адаптер Ethernet:
   IPv4-адрес . . . . . . . . : 192.168.1.50
   Маска подсети . . . . . . . : 255.255.255.0
```

---
