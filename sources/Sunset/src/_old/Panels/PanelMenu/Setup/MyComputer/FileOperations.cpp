#include "defines.h"
#include "FileOperations.h"


FileOperationResult FileOperations::CopyDirectory(const wxString &srcPath, const wxString &destPath, wxWindow *parent)
{
    if (!wxDirExists(srcPath))
    {
        return FileOperationResult::Error("Исходная папка не существует: " + srcPath);
    }

    if (!CheckDiskSpace(srcPath, destPath))
    {
        return FileOperationResult::Error("Недостаточно места на целевом диске");
    }

    if (destPath.StartsWith(srcPath))
    {
        return FileOperationResult::Error("Нельзя копировать папку в саму себя или её подпапку");
    }

    wxProgressDialog *progress = nullptr;
    if (parent)
    {
        progress = new wxProgressDialog("Копирование", "Подготовка...", 100, parent,
            wxPD_CAN_ABORT | wxPD_AUTO_HIDE | wxPD_APP_MODAL);
    }

    int fileCount = 0;
    FileOperationResult result = RecursiveCopyImpl(srcPath, destPath, progress, &fileCount);

    if (progress)
    {
        progress->Destroy();
    }

    return result;
}

FileOperationResult FileOperations::RecursiveCopyImpl(const wxString &srcPath, const wxString &destPath,
    wxProgressDialog *progress, int *fileCount)
{
    if (!wxDirExists(srcPath))
    {
        return FileOperationResult::Error("Исходная папка не существует");
    }

    if (!wxDirExists(destPath))
    {
        if (!wxMkdir(destPath))
        {
            return FileOperationResult::Error("Не удалось создать целевую папку: " + destPath);
        }
    }

    wxDir dir(srcPath);
    if (!dir.IsOpened())
    {
        return FileOperationResult::Error("Не удалось открыть исходную папку");
    }

    int filesProcessed = 0;
    int dirsProcessed = 1; // Считаем текущую директорию
    wxString filename;

    // Копируем файлы
    bool cont = dir.GetFirst(&filename, "", wxDIR_FILES);
    while (cont)
    {
        wxString srcFile = srcPath + wxFileName::GetPathSeparator() + filename;
        wxString destFile = destPath + wxFileName::GetPathSeparator() + filename;

        if (progress && !progress->Pulse(wxString::Format("Копирование: %s", filename)))
        {
            return FileOperationResult::Error("Операция прервана пользователем");
        }

        FileOperationResult fileResult = CopyFile(srcFile, destFile, true);
        if (!fileResult.success)
        {
            return fileResult;
        }

        filesProcessed++;
        if (fileCount) (*fileCount)++;

        cont = dir.GetNext(&filename);
    }

    // Рекурсивно копируем подпапки
    cont = dir.GetFirst(&filename, "", wxDIR_DIRS);
    while (cont)
    {
        wxString srcSubDir = srcPath + wxFileName::GetPathSeparator() + filename;
        wxString destSubDir = destPath + wxFileName::GetPathSeparator() + filename;

        if (progress && !progress->Pulse(wxString::Format("Обработка папки: %s", filename)))
        {
            return FileOperationResult::Error("Операция прервана пользователем");
        }

        FileOperationResult dirResult = RecursiveCopyImpl(srcSubDir, destSubDir, progress, fileCount);
        if (!dirResult.success)
        {
            return dirResult;
        }

        filesProcessed += dirResult.filesProcessed;
        dirsProcessed += dirResult.dirsProcessed;
        cont = dir.GetNext(&filename);
    }

    return FileOperationResult::Success(filesProcessed, dirsProcessed);
}

FileOperationResult FileOperations::DeleteDirectory(const wxString &dirPath, wxWindow *parent)
{
    if (!wxDirExists(dirPath))
    {
        return FileOperationResult::Success(0, 0); // Папка уже не существует
    }

    wxProgressDialog *progress = nullptr;
    if (parent)
    {
        progress = new wxProgressDialog("Удаление", "Подготовка...", 100, parent,
            wxPD_CAN_ABORT | wxPD_AUTO_HIDE | wxPD_APP_MODAL);
    }

    FileOperationResult result = RecursiveDeleteImpl(dirPath);

    if (progress)
    {
        progress->Destroy();
    }

    return result;
}

FileOperationResult FileOperations::RecursiveDeleteImpl(const wxString &dirPath)
{
    wxDir dir(dirPath);
    if (!dir.IsOpened())
    {
        return FileOperationResult::Error("Не удалось открыть папку для удаления");
    }

    int filesDeleted = 0;
    int dirsDeleted = 0;
    wxString filename;

    // Удаляем файлы
    bool cont = dir.GetFirst(&filename, "", wxDIR_FILES);
    while (cont)
    {
        wxString filePath = dirPath + wxFileName::GetPathSeparator() + filename;
        if (!wxRemoveFile(filePath))
        {
            return FileOperationResult::Error("Не удалось удалить файл: " + filename);
        }
        filesDeleted++;
        cont = dir.GetNext(&filename);
    }

    // Рекурсивно удаляем подпапки
    cont = dir.GetFirst(&filename, "", wxDIR_DIRS);
    while (cont)
    {
        wxString subDirPath = dirPath + wxFileName::GetPathSeparator() + filename;
        FileOperationResult subResult = RecursiveDeleteImpl(subDirPath);
        if (!subResult.success)
        {
            return subResult;
        }
        dirsDeleted += subResult.dirsProcessed + 1; // +1 для самой подпапки
        filesDeleted += subResult.filesProcessed;
        cont = dir.GetNext(&filename);
    }

    // Удаляем саму папку
    if (!wxRmdir(dirPath))
    {
        return FileOperationResult::Error("Не удалось удалить папку: " + dirPath);
    }

    return FileOperationResult::Success(filesDeleted, dirsDeleted);
}

// Остальные методы FileOperations...
FileOperationResult FileOperations::MoveDirectory(const wxString &srcPath, const wxString &destPath, wxWindow *parent)
{
    // Быстрое перемещение на одном томе (атомарный rename)
    if (wxRenameFile(srcPath, destPath, false))
    {
        return FileOperationResult::Success(0, 1);
    }

    // Fallback: копирование + удаление (разные тома)
    FileOperationResult copyResult = CopyDirectory(srcPath, destPath, parent);
    if (!copyResult.success)
    {
        return copyResult;
    }

    FileOperationResult deleteResult = DeleteDirectory(srcPath, parent);
    if (!deleteResult.success)
    {
        DeleteDirectory(destPath, parent);
        return FileOperationResult::Error("Не удалось переместить: ошибка при удалении исходной папки");
    }

    return copyResult;
}

FileOperationResult FileOperations::CreateDirectory(const wxString &path, wxWindow * /*parent*/)
{
    if (wxDirExists(path))
    {
        return FileOperationResult::Error("Папка уже существует: " + path);
    }

    if (!wxMkdir(path))
    {
        return FileOperationResult::Error("Не удалось создать папку: " + path);
    }

    return FileOperationResult::Success(0, 1);
}

FileOperationResult FileOperations::CopyFile(const wxString &srcFile, const wxString &destFile, bool overwrite)
{
    if (!wxFileExists(srcFile))
    {
        return FileOperationResult::Error("Исходный файл не существует: " + srcFile);
    }

    if (!wxCopyFile(srcFile, destFile, overwrite))
    {
        return FileOperationResult::Error("Не удалось скопировать файл: " + srcFile);
    }

    return FileOperationResult::Success(1, 0);
}

FileOperationResult FileOperations::MoveFile(const wxString &srcFile, const wxString &destFile, bool overwrite)
{
    if (!wxFileExists(srcFile))
    {
        return FileOperationResult::Error("Исходный файл не существует: " + srcFile);
    }

    if (!wxRenameFile(srcFile, destFile, overwrite))
    {
        return FileOperationResult::Error("Не удалось переместить файл: " + srcFile);
    }

    return FileOperationResult::Success(1, 0);
}

bool FileOperations::CheckDiskSpace(const wxString & /*source*/, const wxString & /*destination*/)
{
    // Упрощенная проверка - в реальном приложении нужно использовать wxGetDiskSpace
    return true;
}

wxString FileOperations::FormatSize(wxULongLong size)
{
    if (size < 1024) return size.ToString() + " B";
    if (size < 1024 * 1024) return wxString::Format("%.1f KB", (double)size.GetValue() / 1024.0);
    if (size < 1024 * 1024 * 1024) return wxString::Format("%.1f MB", (double)size.GetValue() / (1024.0 * 1024.0));
    return wxString::Format("%.1f GB", (double)size.GetValue() / (1024.0 * 1024.0 * 1024.0));
}
