#pragma once
#include "FilePanelController.h"
#include "FilePanelEvents.h"
#include "ClipboardManager.h"


class FTPController;
class FilePanelOperations;


class FilePanel : public wxPanel
{
public:
    // Типы источников данных (переименовано для совместимости)
    enum FileSystemType
    {
        FS_LOCAL,      // Локальная файловая система
        FS_USB,        // USB флешка
        FS_FTP         // FTP соединение
    };

    // Алиасы для обратной совместимости
    enum SourceType
    {
        SOURCE_LOCAL = FS_LOCAL,
        SOURCE_USB = FS_USB,
        SOURCE_FTP = FS_FTP
    };

    // Режим отображения источников
    enum DisplayMode
    {
        MODE_NONE,         // Без выбора источника (для левой панели)
        MODE_COMBOBOX,     // Выпадающий список
        MODE_BUTTONS       // Кнопки (для правой панели)
    };

    // Состояние панели (для правой панели с кнопками)
    enum PanelState
    {
        STATE_SELECTION,   // Показываем только кнопки выбора источника
        STATE_BROWSING     // Показываем каталог и кнопку возврата
    };

    FilePanel(wxWindow *, DisplayMode mode = MODE_NONE);
    virtual ~FilePanel();

    // Удаляем идентификаторы кнопок, оставляем только меню
    enum
    {
        ID_COPY = wxID_HIGHEST + 1,
        ID_MOVE,
        ID_DELETE,
        ID_CREATE_FOLDER,
        ID_REFRESH,
        ID_PASTE,
        ID_SOURCE_TYPE,
        ID_BTN_LOCAL,
        ID_BTN_FTP,
        ID_BTN_BACK,       // Кнопка возврата к выбору источника
        ID_PATH_CTRL,
        ID_BROWSE_BTN,
        ID_FILE_LIST
    };

    void RefreshFileList()
    {
        m_controller->RefreshFileList();
    }
    void SetPath(const wxString &path)
    {
        m_controller->SetPath(path);
    }
    wxString GetCurrentPath() const
    {
        return m_controller->GetCurrentPath();
    }
    wxArrayString GetSelectedFiles() const
    {
        return m_controller->GetSelectedFiles();
    }
    bool HasSelectedFiles() const
    {
        return m_controller->HasSelectedFiles();
    }

    void SetActive(bool active);
    bool IsActive() const
    {
        return m_isActive;
    }

    // Методы для работы с типом источника
    SourceType GetSourceType() const
    {
        return m_sourceType;
    }
    FileSystemType GetFileSystemType() const
    {
        return static_cast<FileSystemType>(m_sourceType);
    }
    void SetSourceType(SourceType type);
    void SetFileSystemType(FileSystemType type)
    {
        SetSourceType(static_cast<SourceType>(type));
    }
    wxString GetSourceTypeString() const;

    // Новые методы для работы с FTP
    bool ConnectToFTP(const wxString &host, int port, const wxString &user, const wxString &pass);
    void DisconnectFTP();
    bool IsFTPConnected() const;
    wxString GetFTPInitialDirectory() const
    {
        return m_ftpInitialDirectory;
    }

    wxTextCtrl *GetPathCtrl() const
    {
        return m_pathCtrl;
    }
    wxListCtrl *GetFileList() const
    {
        return m_fileList;
    }
    void UpdateStatus(const wxString &status) const;

    // Доступ к FTP контроллеру
    FTPController *GetFTPController() const
    {
        return m_ftpController.get();
    }

    // Обработчики операций (делегируют в FilePanelOperations)
    void HandleCopyOperation(wxCommandEvent &event);
    void HandleMoveOperation(wxCommandEvent &event);
    void HandlePasteOperation(wxCommandEvent &event);
    void HandlePasteOperationToTarget(FilePanel *targetPanel);
    void HandleDeleteOperation(wxCommandEvent &event);
    void HandleCreateFolder(wxCommandEvent &event);
    void HandleRefresh(wxCommandEvent &event);
    void HandleUndo();
    void HandleRedo();

    // Доступ к объекту операций
    FilePanelOperations *GetOperations() const
    {
        return m_operations.get();
    }

    // Обработчики событий для контекстного меню
    void OnCopy(wxCommandEvent &event)
    {
        HandleCopyOperation(event);
    }
    void OnMove(wxCommandEvent &event)
    {
        HandleMoveOperation(event);
    }
    void OnPaste(wxCommandEvent &event)
    {
        HandlePasteOperation(event);
    }
    void OnDelete(wxCommandEvent &event)
    {
        HandleDeleteOperation(event);
    }
    void OnCreateFolder(wxCommandEvent &event)
    {
        HandleCreateFolder(event);
    }
    void OnRefresh(wxCommandEvent &event)
    {
        HandleRefresh(event);
    }

    // Обработчики событий
    void OnSourceTypeChanged(wxCommandEvent &event);
    void OnLocalButtonClick(wxCommandEvent &event);
    void OnFTPButtonClick(wxCommandEvent &event);
    void OnBackButtonClick(wxCommandEvent &event);
    void OnPathChanged(wxCommandEvent &event);
    void OnBrowseButton(wxCommandEvent &event);
    void OnItemActivated(wxListEvent &event);
    void OnItemSelected(wxListEvent &event);
    void OnItemRightClick(wxListEvent &event);
    void OnBeginDrag(wxListEvent &event);
    void OnPanelClick(wxMouseEvent &event);
    void OnPanelFocus(wxFocusEvent &event);
    void OnKeyDown(wxKeyEvent &event);
    void OnColumnClick(wxListEvent &event);

private:
    // Удаляем методы CreateButtonPanel и связанные с кнопками элементы
    void CreateControls();
    void CreateSourceButtons();
    void BindEvents();
    void UpdateVisualState();
    void UpdateControlsForSourceType();
    void UpdatePanelState();  // Обновление видимости элементов в зависимости от состояния
    wxArrayString DetectUSBDrives();

    // Внутренние методы для абстракции операций с файловой системой
    bool ChangeDirectoryInternal(const wxString &path);
    wxString GetCurrentDirectoryInternal() const;
    bool GetDirectoryContentsInternal(wxArrayString &files, wxArrayString &dirs);
    bool CreateDirectoryInternal(const wxString &name);
    bool DeleteFileInternal(const wxString &path);
    bool DeleteDirectoryInternal(const wxString &path);
    bool RenameFileInternal(const wxString &oldPath, const wxString &newPath);
    bool FileExistsInternal(const wxString &path) const;
    bool IsDirectoryInternal(const wxString &path) const;
    bool CopyFileBetweenSystems(const wxString &sourcePath, FileSystemType, const wxString &destPath, FileSystemType destType, wxWindow *parent = nullptr);

    DisplayMode m_displayMode;
    PanelState m_panelState;   // Текущее состояние панели
    wxComboBox *m_sourceTypeCombo;
    wxBitmapButton *m_btnLocal;
    wxBitmapButton *m_btnFTP;
    wxButton *m_btnBack;       // Кнопка возврата
    wxBoxSizer *m_buttonsSizer;  // Сайзер для кнопок выбора источника
    wxTextCtrl *m_pathCtrl;
    wxButton *m_browseBtn;
    wxListCtrl *m_fileList;
    // Удаляем указатели на кнопки
    std::unique_ptr<FilePanelController> m_controller;
    std::unique_ptr<FilePanelOperations> m_operations;
    std::unique_ptr<FTPController> m_ftpController;  // Контроллер FTP
    wxString m_ftpInitialDirectory;  // Начальный каталог FTP для ограничения навигации
    bool m_isActive;
    SourceType m_sourceType;
    int m_sortColumn = 0;
    bool m_sortAscending = true;

    wxDECLARE_EVENT_TABLE();
};