#include "defines.h"
#include "FileDropTarget.h"
#include "FilePanelController.h"
#include "FileOperations.h"
#include "ClipboardManager.h"


FileDropTarget::FileDropTarget(FilePanel *panel)
    : m_panel(panel)
{}

bool FileDropTarget::OnDropFiles(wxCoord /*x*/, wxCoord /*y*/, const wxArrayString &filenames)
{
    if (filenames.IsEmpty())
    {
        return false;
    }

    // Получаем исходный путь из первого файла
    wxFileName firstFile(filenames[0]);
    wxString sourcePath = firstFile.GetPath();

    // Создаем данные для буфера обмена
    ClipboardManager::ClipboardData data;
    data.sourcePath = sourcePath;
    data.isCut = false;

    for (const wxString &fullPath : filenames)
    {
        if (fullPath.IsEmpty()) continue;
        wxFileName fn(fullPath);
        data.files.Add(fn.GetFullName());
    }

    // Временно помещаем в буфер и вставляем
    ClipboardManager::GetInstance().SetData(data);
    m_panel->HandlePasteOperationToTarget(m_panel);

    return true;
}
