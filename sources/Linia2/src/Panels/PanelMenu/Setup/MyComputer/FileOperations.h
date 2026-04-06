#pragma once


class FileOperationResult
{
public:
    bool success;
    wxString errorMessage;
    int filesProcessed;
    int dirsProcessed;

    static FileOperationResult Success(int files = 0, int dirs = 0)
    {
        return { true, "", files, dirs };
    }

    static FileOperationResult Error(const wxString &message)
    {
        return { false, message, 0, 0 };
    }
};

class FileOperations
{
public:
    static FileOperationResult CopyDirectory(const wxString &srcPath, const wxString &destPath, wxWindow *parent = nullptr);
    static FileOperationResult MoveDirectory(const wxString &srcPath, const wxString &destPath, wxWindow *parent = nullptr);
    static FileOperationResult DeleteDirectory(const wxString &dirPath, wxWindow *parent = nullptr);
    static FileOperationResult CreateDirectory(const wxString &path, wxWindow *parent = nullptr);
    static FileOperationResult CopyFile(const wxString &srcFile, const wxString &destFile, bool overwrite = true);
    static FileOperationResult MoveFile(const wxString &srcFile, const wxString &destFile, bool overwrite = true);

    static wxString FormatSize(wxULongLong size);

private:
    static FileOperationResult RecursiveCopyImpl(const wxString &srcPath, const wxString &destPath, wxProgressDialog *progress, int *fileCount);
    static FileOperationResult RecursiveDeleteImpl(const wxString &dirPath);
    static bool CheckDiskSpace(const wxString &source, const wxString &destination);
};