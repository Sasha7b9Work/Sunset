#include "defines.h"
#include "FTPController.h"
#include "FilePanel.h"


#ifdef WIN32
#define S_IRUSR 0000400  /* Read permission, owner */
#define S_IWUSR 0000200  /* Write permission, owner */
#define S_IXUSR 0000100  /* Execute permission, owner */
#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)  /* чтение + запись + выполнение */
#endif


FTPController::FTPController(FilePanel *view)
    : m_view(view),
    m_session(nullptr),
    m_ftp(nullptr),
    m_isConnected(false),
    m_currentPath("/")
{}

FTPController::~FTPController()
{
    Disconnect();
}

void FTPController::ParseFTPUrl(const wxString &url, wxString &host, wxString &user,
    wxString &password, wxString &path, int &port)
{
    wxString tempUrl = url;

    // Убираем ftp:// (6 символов)
    if (tempUrl.StartsWith("ftp://"))
    {
        tempUrl = tempUrl.Mid(6);
    }

    // Извлекаем путь
    int pathPos = tempUrl.Find('/');
    if (pathPos != wxNOT_FOUND)
    {
        path = tempUrl.Mid((size_t)pathPos);
        tempUrl = tempUrl.Left((size_t)pathPos);
    }
    else
    {
        path = "/";
    }

    // Извлекаем user:pass@ — ищем ПОСЛЕДНИЙ '@' для защиты от '@' в пароле
    int atPos = tempUrl.Find('@', true);  // true = from end
    if (atPos != wxNOT_FOUND)
    {
        wxString userPass = tempUrl.Left((size_t)atPos);
        tempUrl = tempUrl.Mid((size_t)atPos + 1U);

        // Ищем ПЕРВЫЙ ':' для разделения user:password
        int colonPos = userPass.Find(':');
        if (colonPos != wxNOT_FOUND)
        {
            user = userPass.Left((size_t)colonPos);
            password = userPass.Mid((size_t)colonPos + 1U);
        }
        else
        {
            user = userPass;
            password = "";
        }
    }
    else
    {
        user = wxGetUserId();
        password = "";
    }

    // Извлекаем host:port
    int colonPos = tempUrl.Find(':');
    if (colonPos != wxNOT_FOUND)
    {
        host = tempUrl.Left((size_t)colonPos);
        wxString portStr = tempUrl.Mid((size_t)colonPos + 1U);
        long portLong;
        if (portStr.ToLong(&portLong) && portLong >= 1 && portLong <= 65535)
        {
            port = static_cast<int>(portLong);
        }
        else
        {
            port = 22;
        }
    }
    else
    {
        host = tempUrl;
        port = 22;
    }

    // Валидация: хост не должен быть пустым и не должен содержать спецсимволы
    host = host.Trim().Trim(false);
    if (host.IsEmpty())
    {
        host = "localhost";
    }
}

int FTPController::VerifyKnownHost()
{
    ssh_key srv_pubkey = nullptr;
    unsigned char *hash = nullptr;
    size_t hlen;
    char *hexa;

    int rc = ssh_get_server_publickey(m_session, &srv_pubkey);
    if (rc < 0)
    {
        return -1;
    }

    rc = ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA256, &hash, &hlen);
    ssh_key_free(srv_pubkey);
    if (rc < 0)
    {
        return -1;
    }

    enum ssh_known_hosts_e state = ssh_session_is_known_server(m_session);

    switch (state)
    {
    case SSH_KNOWN_HOSTS_OK:
        ssh_clean_pubkey_hash(&hash);
        return 0;

    case SSH_KNOWN_HOSTS_CHANGED:
        m_lastError = "Внимание: ключ хоста изменился! Возможна атака MITM!";
        ssh_clean_pubkey_hash(&hash);
        return -1;

    case SSH_KNOWN_HOSTS_OTHER:
        m_lastError = "Хост найден, но тип ключа изменился";
        ssh_clean_pubkey_hash(&hash);
        return -1;

    case SSH_KNOWN_HOSTS_NOT_FOUND:
    case SSH_KNOWN_HOSTS_UNKNOWN: {
        hexa = ssh_get_hexa(hash, hlen);
        wxString message = wxString::Format(
            "Хост неизвестен. Отпечаток ключа:\n%s\n\nДоверяете этому хосту?",
            wxString(hexa));
        ssh_string_free_char(hexa);
        ssh_clean_pubkey_hash(&hash);

        int answer = wxMessageBox(message, "Неизвестный хост",
            wxYES_NO | wxICON_QUESTION);
        if (answer == wxYES)
        {
            rc = ssh_session_update_known_hosts(m_session);
            if (rc < 0)
            {
                m_lastError = wxString::Format("Ошибка обновления known_hosts: %s",
                    wxString(ssh_get_error(m_session)));
                return -1;
            }
            return 0;
        }
        return -1;
    }

    case SSH_KNOWN_HOSTS_ERROR:
        m_lastError = wxString::Format("Ошибка проверки known_hosts: %s",
            wxString(ssh_get_error(m_session)));
        ssh_clean_pubkey_hash(&hash);
        return -1;
    }

    ssh_clean_pubkey_hash(&hash);
    return -1;
}

bool FTPController::Connect(const wxString &host, const wxString &user,
    const wxString &password, int port)
{
    if (m_isConnected)
    {
        Disconnect();
    }

    // Создаем SSH сессию
    m_session = ssh_new();
    if (!m_session)
    {
        m_lastError = "Не удалось создать SSH сессию";
        return false;
    }

    // Настраиваем параметры подключения
    ssh_options_set(m_session, SSH_OPTIONS_HOST, host.utf8_str().data());
    ssh_options_set(m_session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(m_session, SSH_OPTIONS_USER, user.utf8_str().data());

    // Устанавливаем таймаут
    long timeout = 10; // 10 секунд
    ssh_options_set(m_session, SSH_OPTIONS_TIMEOUT, &timeout);

    // Подключаемся
    int rc = ssh_connect(m_session);
    if (rc != SSH_OK)
    {
        m_lastError = wxString::Format("Не удалось подключиться: %s",
            wxString(ssh_get_error(m_session)));
        ssh_free(m_session);
        m_session = nullptr;
        return false;
    }

    // Проверяем ключ хоста
    if (VerifyKnownHost() < 0)
    {
        ssh_disconnect(m_session);
        ssh_free(m_session);
        m_session = nullptr;
        return false;
    }

    // Аутентификация
    rc = ssh_userauth_password(m_session, nullptr, password.utf8_str().data());
    if (rc != SSH_AUTH_SUCCESS)
    {
        m_lastError = wxString::Format("Ошибка аутентификации: %s",
            wxString(ssh_get_error(m_session)));
        ssh_disconnect(m_session);
        ssh_free(m_session);
        m_session = nullptr;
        return false;
    }

    // Создаем FTP сессию
    m_ftp = sftp_new(m_session);
    if (!m_ftp)
    {
        m_lastError = wxString::Format("Не удалось создать FTP сессию: %s",
            wxString(ssh_get_error(m_session)));
        ssh_disconnect(m_session);
        ssh_free(m_session);
        m_session = nullptr;
        return false;
    }

    rc = sftp_init(m_ftp);
    if (rc != SSH_OK)
    {
        m_lastError = wxString::Format("Ошибка инициализации FTP: %s",
            wxString(ssh_get_error(m_session)));
        sftp_free(m_ftp);
        m_ftp = nullptr;
        ssh_disconnect(m_session);
        ssh_free(m_session);
        m_session = nullptr;
        return false;
    }

    // Получаем реальный текущий каталог (используем canonicalize для получения абсолютного пути)
    char *realPath = sftp_canonicalize_path(m_ftp, ".");
    if (realPath)
    {
        m_currentPath = wxString::FromUTF8(realPath);
        m_initialPath = m_currentPath;  // Сохраняем начальный путь
        ssh_string_free_char(realPath);
    }
    else
    {
        m_currentPath = "/";
        m_initialPath = "/";
    }

    m_isConnected = true;
    m_lastError.Clear();

    wxLogDebug("FTPController::Connect - current directory: %s", m_currentPath);
    wxLogDebug("FTPController::Connect - initial directory: %s", m_initialPath);

    return true;
}

void FTPController::Disconnect()
{
    if (m_ftp)
    {
        sftp_free(m_ftp);
        m_ftp = nullptr;
    }

    if (m_session)
    {
        ssh_disconnect(m_session);
        ssh_free(m_session);
        m_session = nullptr;
    }

    m_currentPath.Clear();
    m_initialPath.Clear();
    m_isConnected = false;
}

wxString FTPController::BuildFullPath(const wxString &name) const
{
    if (name.StartsWith("/"))
    {
        return name;
    }
    if (m_currentPath.EndsWith("/"))
    {
        return m_currentPath + name;
    }
    return m_currentPath + "/" + name;
}

bool FTPController::ChangeDirectory(const wxString &path)
{
    if (!m_isConnected || !m_ftp)
    {
        m_lastError = "Нет подключения к FTP";
        return false;
    }

    wxLogDebug("FTPController::ChangeDirectory: attempting to change to '%s'", path);
    wxLogDebug("  Current: '%s', Initial: '%s'", m_currentPath, m_initialPath);

    // Проверяем, что новый путь начинается с начального пути
    if (!path.StartsWith(m_initialPath))
    {
        wxLogDebug("  -> BLOCKED: path does not start with initial directory");
        m_lastError = "Навигация за пределы начального каталога запрещена";
        return false;
    }

    // Проверяем, что путь не короче начального (не выше в иерархии)
    if (path.Length() < m_initialPath.Length())
    {
        wxLogDebug("  -> BLOCKED: path is shorter than initial directory");
        m_lastError = "Навигация за пределы начального каталога запрещена";
        return false;
    }

    // Проверяем, существует ли директория
    sftp_dir dir = sftp_opendir(m_ftp, path.utf8_str().data());
    if (!dir)
    {
        m_lastError = wxString::Format("Не удалось открыть директорию: %s",
            wxString(ssh_get_error(m_session)));
        wxLogDebug("  -> FAILED: %s", m_lastError);
        return false;
    }

    sftp_closedir(dir);
    m_currentPath = path;
    wxLogDebug("  -> SUCCESS: changed to '%s'", m_currentPath);
    return true;
}

wxString FTPController::GetCurrentDirectory() const
{
    return m_currentPath;
}

wxArrayString FTPController::ListFiles()
{
    wxArrayString files;

    if (!m_isConnected || !m_ftp)
    {
        m_lastError = "Нет подключения к FTP";
        return files;
    }

    sftp_dir dir = sftp_opendir(m_ftp, m_currentPath.utf8_str().data());
    if (!dir)
    {
        m_lastError = wxString::Format("Не удалось открыть директорию: %s",
            wxString(ssh_get_error(m_session)));
        return files;
    }

    sftp_attributes attr;
    while ((attr = sftp_readdir(m_ftp, dir)) != nullptr)
    {
        wxString name = wxString::FromUTF8(attr->name);

        // Пропускаем . и .. и директории
        if (name != "." && name != ".." && attr->type != SSH_FILEXFER_TYPE_DIRECTORY)
        {
            files.Add(name);
        }

        sftp_attributes_free(attr);
    }

    if (!sftp_dir_eof(dir))
    {
        m_lastError = "Ошибка чтения директории";
    }

    sftp_closedir(dir);
    return files;
}

wxArrayString FTPController::ListDirectories()
{
    wxArrayString dirs;

    if (!m_isConnected || !m_ftp)
    {
        m_lastError = "Нет подключения к FTP";
        return dirs;
    }

    sftp_dir dir = sftp_opendir(m_ftp, m_currentPath.utf8_str().data());
    if (!dir)
    {
        m_lastError = wxString::Format("Не удалось открыть директорию: %s",
            wxString(ssh_get_error(m_session)));
        return dirs;
    }

    sftp_attributes attr;
    while ((attr = sftp_readdir(m_ftp, dir)) != nullptr)
    {
        wxString name = wxString::FromUTF8(attr->name);

        // Пропускаем . и .. , но добавляем все директории
        // Также пропускаем диски Windows (C:, D:, и т.д.)
        bool isWindowsDrive = (name.Length() == 2 && name[1] == ':' &&
            wxIsalpha(name[0]));

        if (name != "." && name != ".." && !isWindowsDrive &&
            attr->type == SSH_FILEXFER_TYPE_DIRECTORY)
        {
            dirs.Add(name);
        }

        sftp_attributes_free(attr);
    }

    if (!sftp_dir_eof(dir))
    {
        m_lastError = "Ошибка чтения директории";
    }

    sftp_closedir(dir);
    return dirs;
}

bool FTPController::DownloadFile(const wxString &remoteFile, const wxString &localFile)
{
    if (!m_isConnected || !m_ftp)
    {
        m_lastError = "Нет подключения к FTP";
        return false;
    }

    wxString fullPath = BuildFullPath(remoteFile);
    // Открываем удаленный файл
    sftp_file file = sftp_open(m_ftp, fullPath.utf8_str().data(), O_RDONLY, 0);
    if (!file)
    {
        m_lastError = wxString::Format("Не удалось открыть удаленный файл: %s",
            wxString(ssh_get_error(m_session)));
        return false;
    }

    // Открываем локальный файл
    wxFileOutputStream output(localFile);
    if (!output.IsOk())
    {
        m_lastError = "Не удалось создать локальный файл";
        sftp_close(file);
        return false;
    }

    // Читаем и записываем данные порциями
    char buffer[16384];
    ssize_t nbytes;
    bool success = true;

    while ((nbytes = sftp_read(file, buffer, sizeof(buffer))) > 0)
    {
        output.Write(buffer, (size_t)nbytes);
        if (!output.IsOk())
        {
            m_lastError = "Ошибка записи в локальный файл";
            success = false;
            break;
        }
    }

    if (nbytes < 0)
    {
        m_lastError = wxString::Format("Ошибка чтения удаленного файла: %s",
            wxString(ssh_get_error(m_session)));
        success = false;
    }

    sftp_close(file);
    return success;
}

bool FTPController::UploadFile(const wxString &localFile, const wxString &remoteFile)
{
    if (!m_isConnected || !m_ftp)
    {
        m_lastError = "Нет подключения к FTP";
        return false;
    }

    // Открываем локальный файл
    wxFileInputStream input(localFile);
    if (!input.IsOk())
    {
        m_lastError = "Не удалось открыть локальный файл";
        return false;
    }

    wxString fullPath = BuildFullPath(remoteFile);
    // Создаем удаленный файл
    sftp_file file = sftp_open(m_ftp, fullPath.utf8_str().data(),
        O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (!file)
    {
        m_lastError = wxString::Format("Не удалось создать удаленный файл: %s",
            wxString(ssh_get_error(m_session)));
        return false;
    }

    // Читаем и записываем данные порциями
    char buffer[16384];
    bool success = true;

    while (!input.Eof())
    {
        input.Read(buffer, sizeof(buffer));
        size_t bytesRead = input.LastRead();

        if (bytesRead > 0)
        {
            ssize_t nwritten = sftp_write(file, buffer, bytesRead);
            if (nwritten != (ssize_t)bytesRead)
            {
                m_lastError = wxString::Format("Ошибка записи в удаленный файл: %s",
                    wxString(ssh_get_error(m_session)));
                success = false;
                break;
            }
        }
    }

    sftp_close(file);
    return success;
}

bool FTPController::DeleteFile(const wxString &remoteFile)
{
    if (!m_isConnected || !m_ftp)
    {
        m_lastError = "Нет подключения к FTP";
        return false;
    }

    wxString fullPath = BuildFullPath(remoteFile);
    int rc = sftp_unlink(m_ftp, fullPath.utf8_str().data());
    if (rc != SSH_OK)
    {
        m_lastError = wxString::Format("Не удалось удалить файл: %s",
            wxString(ssh_get_error(m_session)));
        return false;
    }

    return true;
}

bool FTPController::RenameFile(const wxString &oldName, const wxString &newName)
{
    if (!m_isConnected || !m_ftp)
    {
        m_lastError = "Нет подключения к FTP";
        return false;
    }

    wxString fullOld = BuildFullPath(oldName);
    wxString fullNew = BuildFullPath(newName);
    int rc = sftp_rename(m_ftp, fullOld.utf8_str().data(), fullNew.utf8_str().data());
    if (rc != SSH_OK)
    {
        m_lastError = wxString::Format("Не удалось переименовать: %s",
            wxString(ssh_get_error(m_session)));
        return false;
    }

    return true;
}

bool FTPController::MakeDirectory(const wxString &dirName)
{
    if (!m_isConnected || !m_ftp)
    {
        m_lastError = "Нет подключения к FTP";
        return false;
    }

    wxString fullPath = BuildFullPath(dirName);
    int rc = sftp_mkdir(m_ftp, fullPath.utf8_str().data(), S_IRWXU);
    if (rc != SSH_OK)
    {
        m_lastError = wxString::Format("Не удалось создать директорию: %s",
            wxString(ssh_get_error(m_session)));
        return false;
    }

    return true;
}

bool FTPController::RemoveDirectory(const wxString &dirName)
{
    if (!m_isConnected || !m_ftp)
    {
        m_lastError = "Нет подключения к FTP";
        return false;
    }

    wxString fullPath = BuildFullPath(dirName);
    int rc = sftp_rmdir(m_ftp, fullPath.utf8_str().data());
    if (rc != SSH_OK)
    {
        m_lastError = wxString::Format("Не удалось удалить директорию: %s",
            wxString(ssh_get_error(m_session)));
        return false;
    }

    return true;
}

bool FTPController::IsDirectory(const wxString &path) const
{
    if (!m_isConnected || !m_ftp)
    {
        return false;
    }

    wxString fullPath = path.StartsWith("/") ? path : BuildFullPath(path);
    sftp_attributes attr = sftp_stat(m_ftp, fullPath.utf8_str().data());
    if (!attr)
    {
        return false;
    }

    bool isDir = (attr->type == SSH_FILEXFER_TYPE_DIRECTORY);
    sftp_attributes_free(attr);
    return isDir;
}

wxULongLong FTPController::GetFileSize(const wxString &filename)
{
    if (!m_isConnected || !m_ftp)
    {
        return 0;
    }

    wxString fullPath = BuildFullPath(filename);

    sftp_attributes attr = sftp_stat(m_ftp, fullPath.utf8_str().data());
    if (!attr)
    {
        return 0;
    }

    wxULongLong size = attr->size;
    sftp_attributes_free(attr);

    return size;
}

wxDateTime FTPController::GetFileModTime(const wxString &filename)
{
    wxDateTime dt;

    if (!m_isConnected || !m_ftp)
    {
        return dt;
    }

    wxString fullPath = BuildFullPath(filename);

    sftp_attributes attr = sftp_stat(m_ftp, fullPath.utf8_str().data());
    if (!attr)
    {
        return dt;
    }

    dt = wxDateTime((time_t)attr->mtime);
    sftp_attributes_free(attr);

    return dt;
}
