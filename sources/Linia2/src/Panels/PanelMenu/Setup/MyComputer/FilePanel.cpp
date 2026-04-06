#include "defines.h"
#include "FilePanel.h"
#include "FilePanelEvents.h" 
#include "FilePanelController.h"
#include "FilePanelOperations.h"
#include "FileDropTarget.h"
#include "FileOperations.h"
#include "ClipboardManager.h"
#include "UndoManager.h"
#include "FTPConnectionDialog.h"
#include "FTPController.h"
#include "Controls/Bitmap.h"


#ifdef __WXMSW__
#include <windows.h>
#endif


// Определение таблицы событий
wxBEGIN_EVENT_TABLE(FilePanel, wxPanel)
EVT_COMBOBOX(ID_SOURCE_TYPE, FilePanel::OnSourceTypeChanged)
EVT_BUTTON(ID_BTN_LOCAL, FilePanel::OnLocalButtonClick)
EVT_BUTTON(ID_BTN_FTP, FilePanel::OnFTPButtonClick)
EVT_BUTTON(ID_BTN_BACK, FilePanel::OnBackButtonClick)
EVT_TEXT(ID_PATH_CTRL, FilePanel::OnPathChanged)
EVT_BUTTON(ID_BROWSE_BTN, FilePanel::OnBrowseButton)
EVT_LIST_ITEM_ACTIVATED(ID_FILE_LIST, FilePanel::OnItemActivated)
EVT_LIST_ITEM_SELECTED(ID_FILE_LIST, FilePanel::OnItemSelected)
EVT_LIST_ITEM_RIGHT_CLICK(ID_FILE_LIST, FilePanel::OnItemRightClick)
EVT_LIST_BEGIN_DRAG(ID_FILE_LIST, FilePanel::OnBeginDrag)
EVT_LIST_COL_CLICK(ID_FILE_LIST, FilePanel::OnColumnClick)
EVT_LEFT_DOWN(FilePanel::OnPanelClick)
EVT_SET_FOCUS(FilePanel::OnPanelFocus)
// ТОЛЬКО обработчики меню - удалить дублирующиеся EVT_BUTTON
EVT_MENU(ID_COPY, FilePanel::OnCopy)
EVT_MENU(ID_MOVE, FilePanel::OnMove)
EVT_MENU(ID_PASTE, FilePanel::OnPaste)
EVT_MENU(ID_DELETE, FilePanel::OnDelete)
EVT_MENU(ID_CREATE_FOLDER, FilePanel::OnCreateFolder)
EVT_MENU(ID_REFRESH, FilePanel::OnRefresh)
EVT_CHAR_HOOK(FilePanel::OnKeyDown)
wxEND_EVENT_TABLE()

FilePanel::FilePanel(wxWindow *parent, DisplayMode mode)
    : wxPanel(parent, wxID_ANY),
    m_displayMode(mode),
    m_panelState(STATE_SELECTION),  // Начинаем с экрана выбора источника
    m_sourceTypeCombo(nullptr),
    m_btnLocal(nullptr),
    m_btnFTP(nullptr),
    m_btnBack(nullptr),
    m_buttonsSizer(nullptr),
    m_pathCtrl(nullptr),
    m_browseBtn(nullptr),
    m_fileList(nullptr),
    m_controller(std::make_unique<FilePanelController>(this)),
    m_operations(std::make_unique<FilePanelOperations>(this)),
    m_ftpController(nullptr),
    m_isActive(false),
    m_sourceType(SOURCE_LOCAL)
{
    CreateControls();
    BindEvents();
    UpdateVisualState();

    // Для MODE_BUTTONS начинаем с экрана выбора (только кнопки)
    if (m_displayMode == MODE_BUTTONS)
    {
        UpdatePanelState();
    }
    else
    {
        // Для других режимов инициализируем список файлов
        m_controller->RefreshFileList();
    }
}

FilePanel::~FilePanel()
{
    // Только отключаем FTP-соединение, не обновляя UI (виджеты могут быть уже разрушены)
    if (m_ftpController)
    {
        m_ftpController->Disconnect();
    }
    // Ресурсы (m_controller, m_ftpController) освобождаются автоматически через std::unique_ptr
}

bool FilePanel::ConnectToFTP(const wxString &host, int port,
    const wxString &user, const wxString &pass)
{
    if (!m_ftpController)
    {
        m_ftpController = std::make_unique<FTPController>(this);
    }

    if (!m_ftpController->Connect(host, user, pass, port))
    {
        UpdateStatus("Ошибка подключения к FTP: " + m_ftpController->GetLastError());
        return false;
    }

    m_sourceType = SOURCE_FTP;
    wxString ftpPath = m_ftpController->GetCurrentDirectory();

    // Сохраняем начальный каталог для ограничения навигации
    m_ftpInitialDirectory = ftpPath;
    wxLogDebug("FTP connected, initial directory: %s", m_ftpInitialDirectory);

    if (!ftpPath.IsEmpty())
    {
        m_pathCtrl->SetValue(ftpPath);
    }

    RefreshFileList();
    UpdateStatus("Подключено к FTP: " + host);
    return true;
}

void FilePanel::DisconnectFTP()
{
    if (m_ftpController)
    {
        m_ftpController->Disconnect();
        m_ftpController.reset();
        m_ftpInitialDirectory.Clear();
        m_sourceType = SOURCE_LOCAL;
        m_controller->SetPath(wxGetCwd());
        RefreshFileList();
        UpdateStatus("Отключено от FTP");
    }
}

bool FilePanel::IsFTPConnected() const
{
    return m_ftpController && m_ftpController->IsConnected();
}

void FilePanel::SetActive(bool active)
{
    m_isActive = active;
    UpdateVisualState();
    // Обновляем статус при активации панели
    if (m_isActive && m_controller->HasSelectedFiles())
    {
        m_controller->UpdateStatusForSelection();
    }
    else if (m_isActive)
    {
        UpdateStatus("Панель активна, элементы не выбраны");
    }
}

void FilePanel::UpdateStatus(const wxString &status) const
{
    wxWindow *parent = GetParent();
    if (!parent || parent->IsBeingDeleted()) return;

    wxWindow *grandParent = parent->GetParent();
    if (!grandParent || grandParent->IsBeingDeleted()) return;

    wxCommandEvent statusEvent(wxEVT_FILEPANEL_STATUS, GetId());
    statusEvent.SetString(status);
    grandParent->ProcessWindowEvent(statusEvent);
}

void FilePanel::CreateControls()
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Создаем либо ComboBox либо кнопки в зависимости от режима
    if (m_displayMode == MODE_COMBOBOX)
    {
        // Выпадающий список выбора типа источника
        wxBoxSizer *sourceTypeSizer = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText *sourceLabel = new wxStaticText(this, wxID_ANY, "Источник:");
        m_sourceTypeCombo = new wxComboBox(this, ID_SOURCE_TYPE, wxEmptyString,
            wxDefaultPosition, wxDefaultSize,
            0, nullptr, wxCB_READONLY);
        m_sourceTypeCombo->Append("Локальная файловая система");
        m_sourceTypeCombo->Append("USB флешка");
        m_sourceTypeCombo->Append("FTP соединение");
        m_sourceTypeCombo->SetSelection(0);

        sourceTypeSizer->Add(sourceLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
        sourceTypeSizer->Add(m_sourceTypeCombo, 1, wxEXPAND | wxALL, 5);
        mainSizer->Add(sourceTypeSizer, 0, wxEXPAND | wxALL, 0);
    }
    else if (m_displayMode == MODE_BUTTONS)
    {
        // Кнопки для выбора источника (будут показаны по центру)
        CreateSourceButtons();
        m_buttonsSizer = new wxBoxSizer(wxVERTICAL);  // Вертикальный сайзер для центрирования

        // Создаем горизонтальный сайзер для самих кнопок
        wxBoxSizer *buttonsHorzSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonsHorzSizer->Add(m_btnLocal, 0, wxALL, 10);
        buttonsHorzSizer->Add(m_btnFTP, 0, wxALL, 10);

        // Добавляем кнопки в вертикальный сайзер с центрированием
        m_buttonsSizer->AddStretchSpacer(1);  // Пустое пространство сверху
        m_buttonsSizer->Add(buttonsHorzSizer, 0, wxALIGN_CENTER, 0);
        m_buttonsSizer->AddStretchSpacer(1);  // Пустое пространство снизу

        // Добавляем в главный сайзер - занимает все доступное пространство
        mainSizer->Add(m_buttonsSizer, 1, wxEXPAND, 0);
    }
    // Если MODE_NONE - не добавляем ничего

    // Строка пути с кнопкой возврата
    wxBoxSizer *pathSizer = new wxBoxSizer(wxHORIZONTAL);

    // Кнопка возврата (стрелка назад) - только для MODE_BUTTONS
    if (m_displayMode == MODE_BUTTONS)
    {
        wxBitmap &undoBmp = Bitmap::Get("undo.bmp").GetBitmap();
        if (undoBmp.IsOk())
        {
            m_btnBack = new wxBitmapButton(this, ID_BTN_BACK, undoBmp, wxDefaultPosition);
        }
        else
        {
            m_btnBack = new wxButton(this, ID_BTN_BACK, wxString::FromUTF8("\xe2\x86\x90"), wxDefaultPosition);
        }
        m_btnBack->SetToolTip("Вернуться к выбору источника");
        pathSizer->Add(m_btnBack, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    }

    m_pathCtrl = new wxTextCtrl(this, ID_PATH_CTRL, m_controller->GetCurrentPath(),
        wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    {
        wxBitmap &dirBmp = Bitmap::Get("directory_open.bmp").GetBitmap();
        if (dirBmp.IsOk())
        {
            m_browseBtn = new wxBitmapButton(this, ID_BROWSE_BTN, dirBmp, wxDefaultPosition);
        }
        else
        {
            m_browseBtn = new wxButton(this, ID_BROWSE_BTN, "...", wxDefaultPosition);
        }
        m_browseBtn->SetToolTip("Выбрать папку");
    }

    pathSizer->Add(m_pathCtrl, 1, wxEXPAND | wxALL, 5);
    pathSizer->Add(m_browseBtn, 0, wxALL, 5);

    // Добавляем стили для отображения разделителей
    m_fileList = new wxListCtrl(this, ID_FILE_LIST, wxDefaultPosition, wxDefaultSize,
        wxLC_REPORT | wxLC_VRULES | wxLC_HRULES); // wxLC_VRULES добавляет вертикальные линии

    m_fileList->InsertColumn(0, "Имя", wxLIST_FORMAT_LEFT, 180);
    m_fileList->InsertColumn(1, "Размер", wxLIST_FORMAT_LEFT, 80);
    m_fileList->InsertColumn(2, "Тип", wxLIST_FORMAT_LEFT, 80);
    m_fileList->InsertColumn(3, "Изменен", wxLIST_FORMAT_LEFT, 140);

    mainSizer->Add(pathSizer, 0, wxEXPAND | wxALL, 0);
    mainSizer->Add(m_fileList, 1, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);
}
// Удаляем метод CreateButtonPanel полностью

void FilePanel::BindEvents()
{
    // Существующий код...
    m_fileList->SetDropTarget(new FileDropTarget(this));

    // Добавьте обработку фокуса для списка файлов
    m_fileList->Bind(wxEVT_SET_FOCUS, [this](wxFocusEvent &event)
        {
            if (!m_isActive)
            {
                wxCommandEvent *activateEvent = new wxCommandEvent(wxEVT_FILEPANEL_ACTIVATED, GetId());
                activateEvent->SetEventObject(this);
                GetParent()->GetEventHandler()->QueueEvent(activateEvent);
            }
            event.Skip();
        });
}

void FilePanel::UpdateVisualState()
{
    // Изменяем цвет фона для активной/неактивной панели
    wxColour bgColour = m_isActive ? wxColour(240, 240, 255) : wxColour(255, 255, 255);
    SetBackgroundColour(bgColour);
    m_fileList->SetBackgroundColour(bgColour);
    m_pathCtrl->SetBackgroundColour(bgColour);
    Refresh();
}

// Внутренние методы для абстракции файловых операций
bool FilePanel::ChangeDirectoryInternal(const wxString &path)
{
    switch (m_sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        if (wxDirExists(path))
        {
            return true;
        }
        return false;

    case SOURCE_FTP:
        if (m_ftpController && m_ftpController->ChangeDirectory(path))
        {
            return true;
        }
        return false;
    }
    return false;
}

wxString FilePanel::GetCurrentDirectoryInternal() const
{
    switch (m_sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        return m_controller->GetCurrentPath();

    case SOURCE_FTP:
        if (m_ftpController)
        {
            return m_ftpController->GetCurrentDirectory();
        }
        return wxEmptyString;
    }
    return m_controller->GetCurrentPath();
}

bool FilePanel::GetDirectoryContentsInternal(wxArrayString &files, wxArrayString &dirs)
{
    files.Clear();
    dirs.Clear();

    switch (m_sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB: {
        wxString currentPath = m_controller->GetCurrentPath();
        wxDir dir(currentPath);
        if (!dir.IsOpened()) return false;

        wxString filename;
        bool cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_DIRS);
        while (cont)
        {
            dirs.Add(filename);
            cont = dir.GetNext(&filename);
        }

        cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_FILES);
        while (cont)
        {
            files.Add(filename);
            cont = dir.GetNext(&filename);
        }
        return true;
    }

    case SOURCE_FTP: {
        if (!m_ftpController) return false;

        dirs = m_ftpController->ListDirectories();
        files = m_ftpController->ListFiles();
        return true;
    }
    }
    return false;
}

bool FilePanel::CreateDirectoryInternal(const wxString &name)
{
    switch (m_sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB: {
        wxString fullPath = wxFileName(m_controller->GetCurrentPath(), name).GetFullPath();
        return wxMkdir(fullPath);
    }

    case SOURCE_FTP:
        return m_ftpController && m_ftpController->MakeDirectory(name);
    }
    return false;
}

bool FilePanel::DeleteFileInternal(const wxString &path)
{
    switch (m_sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        return wxRemoveFile(path);

    case SOURCE_FTP:
        return m_ftpController && m_ftpController->DeleteFile(path);
    }
    return false;
}

bool FilePanel::DeleteDirectoryInternal(const wxString &path)
{
    switch (m_sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB: {
        // Рекурсивное удаление
        wxDir dir(path);
        if (!dir.IsOpened()) return false;

        wxString filename;
        bool cont = dir.GetFirst(&filename);
        while (cont)
        {
            wxString fullPath = wxFileName(path, filename).GetFullPath();
            if (wxDirExists(fullPath))
            {
                if (!DeleteDirectoryInternal(fullPath)) return false;
            }
            else
            {
                if (!wxRemoveFile(fullPath)) return false;
            }
            cont = dir.GetNext(&filename);
        }
        return wxRmdir(path);
    }

    case SOURCE_FTP:
        return m_ftpController && m_ftpController->RemoveDirectory(path);
    }
    return false;
}

bool FilePanel::RenameFileInternal(const wxString &oldPath, const wxString &newPath)
{
    switch (m_sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        return wxRenameFile(oldPath, newPath);

    case SOURCE_FTP:
        return m_ftpController && m_ftpController->RenameFile(oldPath, newPath);
    }
    return false;
}

bool FilePanel::FileExistsInternal(const wxString &path) const
{
    switch (m_sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        return wxFileExists(path) || wxDirExists(path);

    case SOURCE_FTP: {
        if (!m_ftpController) return false;
        // Для FTP проверяем через размер файла
        wxULongLong size = m_ftpController->GetFileSize(path);
        return size != wxInvalidSize;
    }
    }
    return false;
}

bool FilePanel::IsDirectoryInternal(const wxString &path) const
{
    switch (m_sourceType)
    {
    case SOURCE_LOCAL:
    case SOURCE_USB:
        return wxDirExists(path);

    case SOURCE_FTP: {
        if (!m_ftpController) return false;
        return m_ftpController->IsDirectory(path);
    }
    }
    return false;
}

bool FilePanel::CopyFileBetweenSystems(const wxString &sourcePath,
    FileSystemType sourceType,
    const wxString &destPath,
    FileSystemType destType,
    wxWindow * /*parent*/)
{
    // Local -> Local
    if (sourceType == FS_LOCAL && destType == FS_LOCAL)
    {
        return wxCopyFile(sourcePath, destPath);
    }

    // Local -> FTP
    if (sourceType == FS_LOCAL && destType == FS_FTP)
    {
        if (!m_ftpController) return false;
        return m_ftpController->UploadFile(sourcePath, destPath);
    }

    // FTP -> Local
    if (sourceType == FS_FTP && destType == FS_LOCAL)
    {
        if (!m_ftpController) return false;
        return m_ftpController->DownloadFile(sourcePath, destPath);
    }

    // FTP -> FTP
    if (sourceType == FS_FTP && destType == FS_FTP)
    {
        // Через временный файл
        wxString tempFile = wxFileName::CreateTempFileName("ftp_copy");

        bool success = m_ftpController->DownloadFile(sourcePath, tempFile);
        if (success)
        {
            success = m_ftpController->UploadFile(tempFile, destPath);
        }

        wxRemoveFile(tempFile);
        return success;
    }

    return false;
}

void FilePanel::OnPathChanged(wxCommandEvent & /*event*/)
{
    wxString newPath = m_pathCtrl->GetValue();
    if (newPath.IsEmpty()) return;  // Игнорируем пустой путь
    m_controller->OnPathChanged(newPath);
}

void FilePanel::OnBrowseButton(wxCommandEvent & /*event*/)
{
    // Открываем диалог выбора директории
    wxDirDialog dlg(this, "Выберите папку", m_controller->GetCurrentPath(), wxDD_DEFAULT_STYLE);
    if (dlg.ShowModal() == wxID_OK)
    {
        m_controller->SetPath(dlg.GetPath());
    }
}

void FilePanel::OnItemActivated(wxListEvent &event)
{
    // Двойной клик по элементу списка (файл или папка)
    m_controller->OnItemActivated(event.GetIndex());
}

void FilePanel::OnItemSelected(wxListEvent &event)
{
    // Активируем панель при выборе элемента
    if (!m_isActive)
    {
        wxCommandEvent *activateEvent = new wxCommandEvent(wxEVT_FILEPANEL_ACTIVATED, GetId());
        activateEvent->SetEventObject(this);
        GetParent()->GetEventHandler()->QueueEvent(activateEvent);
    }
    m_controller->OnItemSelected(event.GetIndex());
}

void FilePanel::OnItemRightClick(wxListEvent & /*event*/)
{
    wxMenu menu;

    auto appendWithIcon = [&](wxMenu &m, int id, const wxString &label, const wxString &iconFile)
        {
            wxMenuItem *item = new wxMenuItem(&m, id, label);
            wxBitmap &bmp = Bitmap::Get(iconFile).GetBitmap();
            if (bmp.IsOk())
            {
                wxImage img = bmp.ConvertToImage();
                img.Rescale(16, 16, wxIMAGE_QUALITY_HIGH);
                item->SetBitmap(wxBitmap(img));
            }
            m.Append(item);
        };

    appendWithIcon(menu, ID_COPY, "Копировать (Ctrl+C)", "edit-copy.bmp");
    appendWithIcon(menu, ID_MOVE, "Вырезать (Ctrl+X)", "edit-cut.bmp");
    appendWithIcon(menu, ID_PASTE, "Вставить (Ctrl+V)", "edit-paste.bmp");
    appendWithIcon(menu, ID_DELETE, "Удалить (Del)", "edit-delete.bmp");
    menu.AppendSeparator();
    appendWithIcon(menu, ID_CREATE_FOLDER, "Создать папку", "folder-new.bmp");
    appendWithIcon(menu, ID_REFRESH, "Обновить", "view-refresh.bmp");

    PopupMenu(&menu);
}

// Обработчики операций - делегируют в FilePanelOperations

void FilePanel::HandleCopyOperation(wxCommandEvent &event)
{
    m_operations->HandleCopyOperation(event);
}

void FilePanel::HandleMoveOperation(wxCommandEvent &event)
{
    m_operations->HandleMoveOperation(event);
}

void FilePanel::HandlePasteOperation(wxCommandEvent &event)
{
    m_operations->HandlePasteOperation(event);
}

void FilePanel::HandlePasteOperationToTarget(FilePanel *targetPanel)
{
    m_operations->HandlePasteOperationToTarget(targetPanel);
}

void FilePanel::HandleDeleteOperation(wxCommandEvent &event)
{
    m_operations->HandleDeleteOperation(event);
}

void FilePanel::HandleCreateFolder(wxCommandEvent &event)
{
    m_operations->HandleCreateFolder(event);
}

void FilePanel::HandleRefresh(wxCommandEvent &event)
{
    m_operations->HandleRefresh(event);
}

void FilePanel::OnBeginDrag(wxListEvent & /*event*/)
{
    // Инициируем drag-and-drop для выбранных файлов
    if (!m_controller->HasSelectedFiles())
    {
        return; // Ничего не выбрано
    }

    wxArrayString selectedFiles = m_controller->GetSelectedFiles();
    if (selectedFiles.IsEmpty())
    {
        return;
    }

    // Создаем wxFileDataObject для совместимости с системным D&D
    wxFileDataObject data;
    for (const wxString &file : selectedFiles)
    {
        if (file != "..")
        {
            wxFileName fullPath(m_controller->GetCurrentPath(), file);
            data.AddFile(fullPath.GetFullPath());
        }
    }

    wxDropSource dragSource(this);
    dragSource.SetData(data);
    dragSource.DoDragDrop(wxDrag_CopyOnly);
}

void FilePanel::OnPanelClick(wxMouseEvent &event)
{
    SetFocus();
    // Отправляем событие активации асинхронно
    wxCommandEvent *activateEvent = new wxCommandEvent(wxEVT_FILEPANEL_ACTIVATED, GetId());
    activateEvent->SetEventObject(this);
    GetParent()->GetEventHandler()->QueueEvent(activateEvent);
    event.Skip();
}

void FilePanel::OnPanelFocus(wxFocusEvent &event)
{
    wxCommandEvent *activateEvent = new wxCommandEvent(wxEVT_FILEPANEL_ACTIVATED, GetId());
    activateEvent->SetEventObject(this);
    GetParent()->GetEventHandler()->QueueEvent(activateEvent);
    event.Skip();
}

void FilePanel::OnKeyDown(wxKeyEvent &event)
{
    if (event.ControlDown())
    {
        int keyCode = event.GetKeyCode();
        wxLogDebug("OnKeyDown: Ctrl + %c (keyCode=%d)", (char)keyCode, keyCode);

        switch (keyCode)
        {
        case 'A':
        case 'a': {
            // Выделить все
            if (m_fileList)
            {
                long count = m_fileList->GetItemCount();
                for (long i = 0; i < count; i++)
                {
                    m_fileList->SetItemState(i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
                }
                UpdateStatus(wxString::Format("Выделено элементов: %ld", count));
            }
            return;
        }
        case 'C':
        case 'c': {
            wxLogDebug("  -> HandleCopyOperation");
            wxCommandEvent evt;
            HandleCopyOperation(evt);
            return;
        }
        case 'X':
        case 'x': {
            wxLogDebug("  -> HandleMoveOperation");
            wxCommandEvent evt;
            HandleMoveOperation(evt);
            return;
        }
        case 'V':
        case 'v': {
            wxLogDebug("  -> HandlePasteOperation");
            wxCommandEvent evt;
            HandlePasteOperation(evt);
            return;
        }
        case 'Z':
        case 'z': {
            HandleUndo();
            return;
        }
        case 'Y':
        case 'y': {
            HandleRedo();
            return;
        }
        }
    }
    else if (event.GetKeyCode() == WXK_DELETE)
    {
        wxCommandEvent evt;
        HandleDeleteOperation(evt);
        return;
    }
    else if (event.GetKeyCode() == WXK_F2)
    {
        // Переименование выделенного файла/папки
        if (!m_fileList || !m_controller)
        {
            event.Skip(); return;
        }

        long sel = m_fileList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (sel == -1)
        {
            event.Skip(); return;
        }

        wxString oldName = m_fileList->GetItemText(sel, 0);
        if (oldName == "..")
        {
            event.Skip(); return;
        }

        wxTextEntryDialog dlg(this, "Новое имя:", "Переименование", oldName);
        if (dlg.ShowModal() != wxID_OK) return;

        wxString newName = dlg.GetValue().Trim().Trim(false);
        if (newName.IsEmpty() || newName == oldName) return;

        wxString basePath = m_controller->GetCurrentPath();
        wxString sep = wxFileName::GetPathSeparator();
        if (m_sourceType == SOURCE_FTP) sep = "/";

        wxString oldPath = basePath + sep + oldName;
        wxString newPath = basePath + sep + newName;

        if (RenameFileInternal(oldPath, newPath))
        {
            m_controller->RefreshFileList();
            UpdateStatus("Переименовано: " + oldName + " → " + newName);
        }
        else
        {
            wxMessageBox("Не удалось переименовать: " + oldName, "Ошибка", wxOK | wxICON_ERROR);
        }
        return;
    }

    event.Skip();
}

void FilePanel::OnColumnClick(wxListEvent &event)
{
    int col = event.GetColumn();
    if (col == m_sortColumn)
    {
        m_sortAscending = !m_sortAscending;
    }
    else
    {
        m_sortColumn = col;
        m_sortAscending = true;
    }

    if (!m_fileList || m_fileList->GetItemCount() <= 1) return;

    // Собираем данные элементов
    struct ItemData {
        wxString cols[4];
        bool isParent;   // ".."
        bool isDir;      // "<DIR>"
    };

    int count = m_fileList->GetItemCount();
    std::vector<ItemData> items((uint64)count);

    for (uint64 i = 0; i < (uint64)count; i++)
    {
        for (int c = 0; c < 4; c++)
        {
            items[i].cols[c] = m_fileList->GetItemText((long)i, c);
        }
        items[i].isParent = (items[i].cols[0] == "..");
        items[i].isDir = (items[i].cols[2] == "<DIR>");
    }

    int sortCol = m_sortColumn;
    bool asc = m_sortAscending;

    std::sort(items.begin(), items.end(), [sortCol, asc](const ItemData &a, const ItemData &b)
        {
            // ".." всегда первый
            if (a.isParent) return true;
            if (b.isParent) return false;
            // Папки перед файлами
            if (a.isDir != b.isDir) return a.isDir;

            int cmp = 0;
            if (sortCol == 1)
            {
                cmp = a.cols[1].CmpNoCase(b.cols[1]);
            }
            else
            {
                cmp = a.cols[sortCol].CmpNoCase(b.cols[sortCol]);
            }
            return asc ? (cmp < 0) : (cmp > 0);
        });

    // Перестраиваем список
    m_fileList->Freeze();
    m_fileList->DeleteAllItems();

    for (uint64 i = 0; i < (uint64)count; i++)
    {
        long item = m_fileList->InsertItem((long)i, items[i].cols[0]);
        m_fileList->SetItem(item, 1, items[i].cols[1]);
        m_fileList->SetItem(item, 2, items[i].cols[2]);
        m_fileList->SetItem(item, 3, items[i].cols[3]);
    }

    m_fileList->Thaw();
}

void FilePanel::HandleUndo()
{
    m_operations->HandleUndo();
}

void FilePanel::HandleRedo()
{
    m_operations->HandleRedo();
}

// Методы для работы с типом источника данных
void FilePanel::SetSourceType(SourceType type)
{
    m_sourceType = type;
    if (m_sourceTypeCombo)
    {
        m_sourceTypeCombo->SetSelection(static_cast<int>(type));
    }
    UpdateControlsForSourceType();
    RefreshFileList();
}

wxString FilePanel::GetSourceTypeString() const
{
    switch (m_sourceType)
    {
    case SOURCE_LOCAL:
        return "Локальная файловая система";
    case SOURCE_USB:
        return "USB флешка";
    case SOURCE_FTP:
        return "FTP соединение";
    default:
        return "Неизвестно";
    }
}

void FilePanel::OnSourceTypeChanged(wxCommandEvent & /*event*/)
{
    int selection = m_sourceTypeCombo->GetSelection();
    m_sourceType = static_cast<SourceType>(selection);

    UpdateStatus(wxString::Format("Переключено на: %s", GetSourceTypeString()));
    UpdateControlsForSourceType();
    RefreshFileList();
}

void FilePanel::UpdateControlsForSourceType()
{
    switch (m_sourceType)
    {
    case SOURCE_LOCAL:
        m_pathCtrl->Enable(true);
        m_browseBtn->Enable(true);
        m_pathCtrl->SetValue(wxGetCwd());
        break;

    case SOURCE_USB: {
        m_pathCtrl->Enable(true);
        m_browseBtn->Enable(true);
        wxArrayString usbDrives = DetectUSBDrives();
        if (usbDrives.Count() > 0)
        {
            m_pathCtrl->SetValue(usbDrives[0]);
            UpdateStatus(wxString::Format("Найдено USB устройств: %d", usbDrives.Count()));
        }
        else
        {
            m_pathCtrl->SetValue("");
            UpdateStatus("USB устройства не обнаружены");
        }
        break;
    }

    case SOURCE_FTP:
        m_pathCtrl->Enable(true);
        m_browseBtn->Enable(false);
        m_pathCtrl->SetValue("ftp://");
        UpdateStatus("Введите FTP адрес (например: ftp://192.168.1.100/)");
        break;
    }
}

wxArrayString FilePanel::DetectUSBDrives()
{
    wxArrayString usbDrives;

#ifdef __WXMSW__
    // Для Windows - сканируем диски от D: до Z:
    for (char drive = 'D'; drive <= 'Z'; drive++)
    {
        wxString drivePath = wxString::Format("%c:\\", drive);
        if (wxDirExists(drivePath))
        {
            // Проверяем, является ли диск съемным устройством
            UINT driveType = GetDriveType(drivePath.wc_str());
            if (driveType == DRIVE_REMOVABLE || driveType == DRIVE_UNKNOWN)
            {
                usbDrives.Add(drivePath);
            }
        }
    }
#else
    // Для Linux - проверяем /media и /mnt
    wxArrayString mediaDirs;
    mediaDirs.Add("/media");
    mediaDirs.Add("/mnt");

    for (size_t i = 0; i < mediaDirs.Count(); i++)
    {
        if (wxDirExists(mediaDirs[i]))
        {
            wxDir dir(mediaDirs[i]);
            if (dir.IsOpened())
            {
                wxString username = wxGetUserId();
                wxString userMediaPath = mediaDirs[i] + "/" + username;

                if (wxDirExists(userMediaPath))
                {
                    wxDir userDir(userMediaPath);
                    wxString filename;
                    bool cont = userDir.GetFirst(&filename, "", wxDIR_DIRS);
                    while (cont)
                    {
                        usbDrives.Add(userMediaPath + "/" + filename);
                        cont = userDir.GetNext(&filename);
                    }
                }
            }
        }
    }
#endif

    return usbDrives;
}

void FilePanel::CreateSourceButtons()
{
    // Загружаем иконки
    wxBitmap &bmpLocal = Bitmap::Get("computer.bmp").GetBitmap();
    wxBitmap &bmpFTP = Bitmap::Get("ftp.bmp").GetBitmap();

    // Создаем кнопки
    if (bmpLocal.IsOk())
    {
        m_btnLocal = new wxBitmapButton(this, ID_BTN_LOCAL, bmpLocal,
            wxDefaultPosition, wxSize(64, 64));
    }
    else
    {
        // Создаём пустой bitmap 64x64 вместо wxNullBitmap (wxNullBitmap может вызвать segfault на GTK)
        wxBitmap emptyBmp(64, 64);
        {
            wxMemoryDC dc(emptyBmp);
            dc.SetBackground(*wxWHITE_BRUSH);
            dc.Clear();
            dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            dc.DrawText("Local", 10, 22);
        }
        m_btnLocal = new wxBitmapButton(this, ID_BTN_LOCAL, emptyBmp,
            wxDefaultPosition, wxSize(64, 64));
    }
    m_btnLocal->SetToolTip("Локальное хранилище");

    if (bmpFTP.IsOk())
    {
        m_btnFTP = new wxBitmapButton(this, ID_BTN_FTP, bmpFTP,
            wxDefaultPosition, wxSize(64, 64));
    }
    else
    {
        wxBitmap emptyBmp(64, 64);
        {
            wxMemoryDC dc(emptyBmp);
            dc.SetBackground(*wxWHITE_BRUSH);
            dc.Clear();
            dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
            dc.DrawText("FTP", 18, 22);
        }
        m_btnFTP = new wxBitmapButton(this, ID_BTN_FTP, emptyBmp,
            wxDefaultPosition, wxSize(64, 64));
    }
    m_btnFTP->SetToolTip("FTP соединение");
}

void FilePanel::OnLocalButtonClick(wxCommandEvent & /*event*/)
{
    m_sourceType = SOURCE_LOCAL;
    m_panelState = STATE_BROWSING;  // Переходим в режим просмотра каталога
    UpdateStatus("Переключено на: Локальное хранилище");
    UpdatePanelState();
    RefreshFileList();
}

void FilePanel::OnFTPButtonClick(wxCommandEvent & /*event*/)
{
    // Показываем диалог подключения к FTP
    FTPConnectionDialog dlg(this);
    if (dlg.ShowModal() == wxID_OK)
    {
        wxString server = dlg.GetServer();
        int port = dlg.GetPort();
        wxString username = dlg.GetUsername();
        wxString password = dlg.GetPassword();

        // Создаем FTP контроллер если его еще нет
        if (!m_ftpController)
        {
            m_ftpController = std::make_unique<FTPController>(this);
        }

        // Пытаемся подключиться
        bool connected = m_ftpController->Connect(server, username, password, port);

        // Очищаем пароль из памяти сразу после использования
        dlg.ClearPassword();
        for (size_t i = 0; i < password.length(); ++i)
        {
            password[i] = '\0';
        }
        password.Clear();

        if (connected)
        {
            m_sourceType = SOURCE_FTP;
            m_panelState = STATE_BROWSING;  // Переходим в режим просмотра каталога

            // Формируем путь для отображения
            wxString ftpPath = wxString::Format("ftp://%s:%d/", server, port);
            m_pathCtrl->SetValue(ftpPath);

            UpdateStatus(wxString::Format("Подключено к FTP: %s", server));
            UpdatePanelState();
            RefreshFileList();
        }
        else
        {
            wxMessageBox("Не удалось подключиться к FTP серверу", "Ошибка подключения",
                wxOK | wxICON_ERROR, this);
        }
    }
}

void FilePanel::OnBackButtonClick(wxCommandEvent & /*event*/)
{
    // Возврат к экрану выбора источника
    m_panelState = STATE_SELECTION;

    // Если был FTP - отключаемся
    if (m_sourceType == SOURCE_FTP && m_ftpController)
    {
        m_ftpController.reset();  // Уничтожаем контроллер, это закроет соединение
        UpdateStatus("FTP соединение закрыто");
    }
    else
    {
        UpdateStatus("Возврат к выбору источника");
    }

    m_sourceType = SOURCE_LOCAL;  // Сбрасываем тип источника
    UpdatePanelState();
}

void FilePanel::UpdatePanelState()
{
    if (m_displayMode != MODE_BUTTONS)
    {
        return;  // Этот метод только для режима кнопок
    }

    bool showButtons = (m_panelState == STATE_SELECTION);
    bool showBrowser = (m_panelState == STATE_BROWSING);

    // Показываем/скрываем кнопки выбора источника
    if (m_buttonsSizer)
    {
        m_buttonsSizer->Show(showButtons);
    }

    // Показываем/скрываем элементы браузера файлов
    if (m_btnBack) m_btnBack->Show(showBrowser);
    if (m_pathCtrl) m_pathCtrl->Show(showBrowser);
    if (m_browseBtn) m_browseBtn->Show(showBrowser);
    if (m_fileList) m_fileList->Show(showBrowser);

    // Обновляем layout
    Layout();
    Refresh();
}
