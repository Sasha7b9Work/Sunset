#pragma once


class FilePanel;


class FilePanelController
{
public:
    explicit FilePanelController(FilePanel *view);

    void RefreshFileList();
    void SetPath(const wxString &path);
    wxString GetCurrentPath() const
    {
        return m_currentPath;
    }
    wxArrayString GetSelectedFiles() const;
    bool HasSelectedFiles() const;

    void OnPathChanged(const wxString &newPath);
    void OnItemActivated(long itemIndex);
    void OnItemSelected(long itemIndex);
    void UpdateStatusForSelection() const;

private:
    void PopulateFileList();
    void PopulateLocalFileList();
    void PopulateFTPFileList();
    void AddDirectoryItem(const wxString &name, const wxString &displayName = "");
    void AddFileItem(const wxFileName &);
    void AddFTPItem(const wxString &name, bool isDir, wxULongLong size = 0);

    FilePanel *m_view;
    wxString m_currentPath;

    bool m_updatingPath = false;
};
