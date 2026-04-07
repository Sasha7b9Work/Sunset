#include "defines.h"
#include "ClipboardManager.h"


ClipboardManager &ClipboardManager::GetInstance()
{
    static ClipboardManager instance;
    return instance;
}

wxString ClipboardManager::GetOperationDescription() const
{
    if (m_data.IsEmpty()) return "Буфер обмена пуст";

    size_t fileCount = 0, dirCount = 0;
    for (const wxString &filename : m_data.files)
    {
        if (filename == "..") continue;
        wxFileName fullPath(m_data.sourcePath, filename);
        if (fullPath.IsDir()) dirCount++;
        else fileCount++;
    }

    wxString op = m_data.isCut ? wxString("Вырезано") : wxString("Скопировано");
    if (fileCount > 0 && dirCount > 0)
    {
        return wxString::Format("%s: %d файлов и %d папок", op, (int)fileCount, (int)dirCount);
    }
    else if (fileCount > 0)
    {
        return wxString::Format("%s файлов: %d", op, (int)fileCount);
    }
    else
    {
        return wxString::Format("%s папок: %d", op, (int)dirCount);
    }
}
