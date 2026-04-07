#include "defines.h"
#include "FilePanelController.h"
#include "FilePanel.h"
#include "FileOperations.h"
#include "FTPController.h"


FilePanelController::FilePanelController(FilePanel *view)
    : m_view(view), m_currentPath(wxGetCwd())
{}

void FilePanelController::RefreshFileList()
{
    PopulateFileList();
}

void FilePanelController::PopulateFileList()
{
    // Проверяем тип источника данных
    if (m_view->GetSourceType() == FilePanel::SOURCE_FTP)
    {
        PopulateFTPFileList();
    }
    else
    {
        PopulateLocalFileList();
    }
}

void FilePanelController::PopulateLocalFileList()
{
    wxListCtrl *fileList = m_view->GetFileList();
    if (!fileList) return;

    fileList->Freeze();
    fileList->DeleteAllItems();

    if (!wxDirExists(m_currentPath))
    {
        fileList->Thaw();
        return;
    }

    try
    {
        wxDir dir(m_currentPath);
        if (!dir.IsOpened())
        {
            fileList->Thaw();
            return;
        }

        // Добавляем ".." для навигации вверх (кроме корневого каталога)
        if (m_currentPath != wxFileName::GetPathSeparator())
        {
            AddDirectoryItem("..");
        }

        // Сначала папки
        wxString filename;
        bool cont = dir.GetFirst(&filename, "", wxDIR_DIRS);
        while (cont)
        {
            AddDirectoryItem(filename);
            cont = dir.GetNext(&filename);
        }

        // Затем файлы
        cont = dir.GetFirst(&filename, "", wxDIR_FILES);
        while (cont)
        {
            wxFileName file(m_currentPath, filename);
            AddFileItem(file);
            cont = dir.GetNext(&filename);
        }
    }
    catch (const std::exception &e)
    {
        wxLogError("Ошибка доступа к каталогу '%s': %s", m_currentPath, wxString(e.what()));
        m_view->UpdateStatus(wxString::Format("Ошибка доступа: %s", wxString(e.what())));
    }
    catch (...)
    {
        wxLogError("Неизвестная ошибка при чтении каталога '%s'", m_currentPath);
        m_view->UpdateStatus("Ошибка чтения каталога");
    }
    fileList->Thaw();
}

void FilePanelController::PopulateFTPFileList()
{
    wxListCtrl *fileList = m_view->GetFileList();
    if (!fileList) return;

    fileList->Freeze();
    fileList->DeleteAllItems();

    // Получаем FTP контроллер из FilePanel
    // Используем метод доступа, который мы создадим
    FTPController *ftpCtrl = m_view->GetFTPController();
    if (!ftpCtrl || !ftpCtrl->IsConnected())
    {
        m_view->UpdateStatus("Нет подключения к FTP");
        fileList->Thaw();
        return;
    }

    // Добавляем ".." для навигации вверх (но не в начальном каталоге)
    wxString currentDir = ftpCtrl->GetCurrentDirectory();
    wxString initialDir = m_view->GetFTPInitialDirectory();

    // Показываем ".." только если мы не в начальном каталоге
    if (!currentDir.IsEmpty() && currentDir != "/" && currentDir != initialDir)
    {
        AddFTPItem("..", true);
    }

    // Получаем список директорий
    wxArrayString dirs = ftpCtrl->ListDirectories();
    for (const wxString &dir : dirs)
    {
        if (!dir.IsEmpty() && dir != "." && dir != "..")
        {
            AddFTPItem(dir, true);
        }
    }

    // Получаем список файлов
    wxArrayString files = ftpCtrl->ListFiles();
    for (const wxString &file : files)
    {
        if (!file.IsEmpty())
        {
            wxULongLong size = ftpCtrl->GetFileSize(file);
            AddFTPItem(file, false, size);
        }
    }

    m_view->UpdateStatus(wxString::Format("Каталог FTP: %s", currentDir));
    fileList->Thaw();
}

void FilePanelController::SetPath(const wxString &path)
{
    if (m_updatingPath) return;  // Предотвращаем рекурсию

    // Для FTP обрабатываем путь отдельно
    if (m_view->GetSourceType() == FilePanel::SOURCE_FTP)
    {
        FTPController *ftpCtrl = m_view->GetFTPController();
        if (ftpCtrl && ftpCtrl->IsConnected())
        {
            m_updatingPath = true;

            // Извлекаем путь из FTP URL
            wxString ftpPath = path;
            if (ftpPath.StartsWith("ftp://"))
            {
                // Находим путь после хоста
                                                 // \todo Здесь ошибка. Нужно пропустить первые 6 символов
                int slashPos = ftpPath.Find('/', false);            // Пропускаем "ftp://"
                if (slashPos != wxNOT_FOUND)
                {
                    // \todo Здесь ошибка.
                    slashPos = ftpPath.Find('/', false);     // Находим следующий слэш
                    //                    slashPos = ftpPath.Find('/', slashPos + 1);     // Находим следующий слэш
                    if (slashPos != wxNOT_FOUND)
                    {
                        ftpPath = ftpPath.Mid((size_t)slashPos);
                    }
                    else
                    {
                        ftpPath = "/";
                    }
                }
                else
                {
                    ftpPath = "/";
                }
            }

            if (ftpCtrl->ChangeDirectory(ftpPath))
            {
                m_currentPath = ftpPath;

                wxTextCtrl *pathCtrl = m_view->GetPathCtrl();
                if (pathCtrl)
                {
                    pathCtrl->ChangeValue(path);
                }

                PopulateFileList();
            }

            m_updatingPath = false;
        }
        return;
    }

    // Локальная файловая система — нормализуем путь для предотвращения path traversal
    wxFileName normalized(path);
    normalized.Normalize(wxPATH_NORM_DOTS | wxPATH_NORM_ABSOLUTE | wxPATH_NORM_TILDE);
    wxString safePath = normalized.GetFullPath();

    if (!wxDirExists(safePath))
    {
        return;  // Не пытаемся установить несуществующий путь
    }

    m_updatingPath = true;
    m_currentPath = safePath;

    wxTextCtrl *pathCtrl = m_view->GetPathCtrl();
    if (pathCtrl && pathCtrl->GetValue() != m_currentPath)
    {
        pathCtrl->ChangeValue(m_currentPath);  // Используем ChangeValue вместо SetValue
    }

    PopulateFileList();
    m_updatingPath = false;
}

void FilePanelController::AddDirectoryItem(const wxString &name, const wxString &displayName)
{
    wxListCtrl *fileList = m_view->GetFileList();
    if (!fileList) return;

    long index = fileList->GetItemCount();
    long item = fileList->InsertItem(index, displayName.IsEmpty() ? name : displayName);
    fileList->SetItem(item, 1, ""); // Размер
    fileList->SetItem(item, 2, "<DIR>"); // Тип

    wxFileName fn(m_currentPath, name);
    if (fn.DirExists())
    {
        wxDateTime modTime;
        fn.GetTimes(nullptr, &modTime, nullptr);
        fileList->SetItem(item, 3, modTime.Format("%d.%m.%Y %H:%M"));
    }
}

void FilePanelController::AddFileItem(const wxFileName &file)
{
    wxListCtrl *fileList = m_view->GetFileList();
    if (!fileList) return;

    long index = fileList->GetItemCount();
    long item = fileList->InsertItem(index, file.GetFullName());

    if (file.FileExists())
    {
        wxULongLong size = file.GetSize();
        fileList->SetItem(item, 1, FileOperations::FormatSize(size));

        wxString ext = file.GetExt();
        fileList->SetItem(item, 2, ext.IsEmpty() ? _("Файл") : ext.Upper());

        wxDateTime modTime;
        file.GetTimes(nullptr, &modTime, nullptr);
        fileList->SetItem(item, 3, modTime.Format("%d.%m.%Y %H:%M"));
    }
}

wxArrayString FilePanelController::GetSelectedFiles() const
{
    wxArrayString files;
    wxListCtrl *fileList = m_view->GetFileList();
    if (!fileList) return files;

    long item = fileList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    while (item != -1)
    {
        files.Add(fileList->GetItemText(item, 0));
        item = fileList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    }

    wxLogDebug("GetSelectedFiles: найдено %d файлов", (int)files.size()); // Отладка
    return files;
}

bool FilePanelController::HasSelectedFiles() const
{
    wxListCtrl *fileList = m_view->GetFileList();
    if (!fileList) return false;

    long itemCount = fileList->GetItemCount();
    for (long i = 0; i < itemCount; i++)
    {
        if (fileList->GetItemState(i, wxLIST_STATE_SELECTED) & wxLIST_STATE_SELECTED)
        {
            return true;
        }
    }
    return false;
}

void FilePanelController::OnPathChanged(const wxString &newPath)
{
    if (m_updatingPath) return;  // Игнорируем, если уже обновляем
    SetPath(newPath);
}

void FilePanelController::OnItemActivated(long itemIndex)
{
    wxListCtrl *fileList = m_view->GetFileList();
    if (!fileList || itemIndex < 0 || itemIndex >= fileList->GetItemCount())
        return;

    wxString filename = fileList->GetItemText(itemIndex, 0);
    wxString type = fileList->GetItemText(itemIndex, 2);

    if (type == "<DIR>" || filename == "..")
    {
        // Для FTP обрабатываем навигацию отдельно
        if (m_view->GetSourceType() == FilePanel::SOURCE_FTP)
        {
            FTPController *ftpCtrl = m_view->GetFTPController();
            if (!ftpCtrl || !ftpCtrl->IsConnected()) return;

            wxString newPath;
            if (filename == "..")
            {
                // Переходим на уровень вверх
                wxString currentDir = ftpCtrl->GetCurrentDirectory();
                wxString initialDir = m_view->GetFTPInitialDirectory();

                wxLogDebug("FTP navigation up: current='%s', initial='%s'", currentDir, initialDir);

                // Запрещаем выходить выше начального каталога
                if (currentDir == initialDir || currentDir.Length() <= initialDir.Length())
                {
                    wxLogDebug("  -> Already at or above initial directory, navigation blocked");
                    m_view->UpdateStatus("Невозможно подняться выше начального каталога");
                    return;
                }

                if (currentDir != "/" && !currentDir.IsEmpty())
                {
                    // Удаляем последний компонент пути
                    int lastSlash = currentDir.Find('/', true);
                    if (lastSlash != wxNOT_FOUND && lastSlash > 0)
                    {
                        newPath = currentDir.Left((size_t)lastSlash);
                    }
                    else
                    {
                        newPath = "/";
                    }

                    // Дополнительная проверка: новый путь не должен быть короче начального
                    if (newPath.Length() < initialDir.Length())
                    {
                        wxLogDebug("  -> New path '%s' would be above initial '%s', blocked", newPath, initialDir);
                        m_view->UpdateStatus("Невозможно подняться выше начального каталога");
                        return;
                    }

                    // Проверяем, что новый путь начинается с начального каталога
                    if (!newPath.StartsWith(initialDir))
                    {
                        wxLogDebug("  -> New path '%s' does not start with initial '%s', blocked", newPath, initialDir);
                        m_view->UpdateStatus("Невозможно подняться выше начального каталога");
                        return;
                    }
                }
            }
            else
            {
                // Переходим в подкаталог
                wxString currentDir = ftpCtrl->GetCurrentDirectory();
                if (currentDir.EndsWith("/"))
                {
                    newPath = currentDir + filename;
                }
                else
                {
                    newPath = currentDir + "/" + filename;
                }
            }

            if (!newPath.IsEmpty() && ftpCtrl->ChangeDirectory(newPath))
            {
                m_currentPath = newPath;

                // Обновляем строку пути в UI
                wxTextCtrl *pathCtrl = m_view->GetPathCtrl();
                if (pathCtrl)
                {
                    // Формируем полный FTP URL для отображения
                    wxString displayPath = pathCtrl->GetValue();
                    // Извлекаем префикс ftp://host:port
                    int pathStart = displayPath.Find('/', false);       // \todo Ошибка в последнем параметре
                    //                    int pathStart = displayPath.Find('/', 6);  // Пропускаем "ftp://"
                    if (pathStart != wxNOT_FOUND)
                    {
                        pathStart = displayPath.Find('/', false);       // \todo Ошибка в последнем параметре
                        //                        pathStart = displayPath.Find('/', pathStart + 1);
                        if (pathStart != wxNOT_FOUND)
                        {
                            displayPath = displayPath.Left((size_t)pathStart) + newPath;
                        }
                    }
                    pathCtrl->ChangeValue(displayPath);
                }

                PopulateFileList();
            }
        }
        else
        {
            // Локальная навигация
            wxString newPath;
            if (filename == "..")
            {
                wxFileName fn(m_currentPath);
                fn.RemoveLastDir();
                newPath = fn.GetPath();
            }
            else
            {
                newPath = m_currentPath + wxFileName::GetPathSeparator() + filename;
            }
            SetPath(newPath);
        }
    }
}

void FilePanelController::OnItemSelected(long /*itemIndex*/)
{
    UpdateStatusForSelection();
}

void FilePanelController::UpdateStatusForSelection() const
{
    wxListCtrl *fileList = m_view->GetFileList();
    if (!fileList) return;

    wxArrayString selected = GetSelectedFiles();
    if (selected.IsEmpty()) return;

    // Для простоты показываем информацию о первом выбранном элементе
    wxString filename = selected[0];
    if (filename == "..") return;

    wxString status;

    // Для FTP не пытаемся обращаться к локальной файловой системе
    if (m_view->GetSourceType() == FilePanel::SOURCE_FTP)
    {
        status = wxString::Format("Выбран: %s", filename);
        m_view->UpdateStatus(status);
        return;
    }

    wxString fullPathStr = m_currentPath + wxFileName::GetPathSeparator() + filename;

    if (wxDirExists(fullPathStr))
    {
        status = wxString::Format("Выбрана папка: %s", filename);
    }
    else if (wxFileExists(fullPathStr))
    {
        wxFileName fn(fullPathStr);
        wxULongLong size = fn.GetSize();
        status = wxString::Format("Выбран файл: %s (%s)", filename, FileOperations::FormatSize(size));
    }
    else
    {
        status = wxString::Format("Выбран: %s", filename);
    }

    m_view->UpdateStatus(status);
}

void FilePanelController::AddFTPItem(const wxString &name, bool isDir, wxULongLong size)
{
    wxListCtrl *fileList = m_view->GetFileList();
    if (!fileList) return;

    long index = fileList->GetItemCount();
    long item = fileList->InsertItem(index, name);

    if (isDir)
    {
        fileList->SetItem(item, 1, "");  // Размер пустой для директории
        fileList->SetItem(item, 2, "<DIR>");
        fileList->SetItem(item, 3, "");  // Дата не доступна через простое API
    }
    else
    {
        fileList->SetItem(item, 1, FileOperations::FormatSize(size));

        wxString ext;
        int dotPos = name.Find('.', true);  // Ищем последнюю точку
        if (dotPos != wxNOT_FOUND)
        {
            ext = name.Mid((size_t)dotPos + 1U).Upper();
        }
        else
        {
            ext = "Файл";
        }
        fileList->SetItem(item, 2, ext);
        fileList->SetItem(item, 3, "");  // Дата не доступна
    }
}
