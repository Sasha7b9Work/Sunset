#pragma once


// Forward declarations
class FilePanel;
class FTPController;

// Объявляем enum вне класса для избежания циклических зависимостей
enum FilePanelSourceType {
    SOURCE_TYPE_LOCAL,
    SOURCE_TYPE_USB,
    SOURCE_TYPE_FTP
};

class ClipboardManager {
public:
    struct ClipboardData {
        wxArrayString files;
        wxString sourcePath;
        bool isCut = false;
        FilePanelSourceType sourceType = SOURCE_TYPE_LOCAL;
        FTPController *sourceController = nullptr;  // Типизированный указатель на FTPController

        bool IsEmpty() const
        {
            return files.IsEmpty();
        }
        void Clear()
        {
            files.Clear();
            sourcePath.Clear();
            isCut = false;
            sourceType = SOURCE_TYPE_LOCAL;
            sourceController = nullptr;
        }
    };

    static ClipboardManager &GetInstance();

    void SetData(const ClipboardData &data)
    {
        m_data = data;
    }
    ClipboardData GetData() const
    {
        return m_data;
    }
    bool HasData() const
    {
        return !m_data.IsEmpty();
    }
    void Clear()
    {
        m_data.Clear();
    }

    wxString GetOperationDescription() const;

private:
    ClipboardManager() = default;
    ~ClipboardManager() = default;

    ClipboardData m_data;
};
