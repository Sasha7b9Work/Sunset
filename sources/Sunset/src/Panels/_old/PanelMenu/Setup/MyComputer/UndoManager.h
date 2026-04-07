#pragma once


enum class OperationType
{
    COPY,
    MOVE,
    _DELETE,
    CREATE_FOLDER
};

struct FileOperation
{
    OperationType type;
    wxArrayString files;
    wxString sourcePath;
    wxString destPath;

    FileOperation(OperationType t, const wxArrayString &f,const wxString &src, const wxString &dst = ""):
        type(t), files(f), sourcePath(src), destPath(dst)
    {
    }
};

class UndoManager
{
public:
    static UndoManager &GetInstance();

    void AddOperation(const FileOperation &operation);
    bool CanUndo() const
    {
        return m_currentIndex > 0;
    }
    bool CanRedo() const
    {
        return m_currentIndex < m_operations.size();
    }

    FileOperation Undo();
    FileOperation Redo();
    void Clear();

private:
    UndoManager() : m_currentIndex(0) { }

    std::vector<FileOperation> m_operations;
    size_t m_currentIndex;
    static const size_t MAX_OPERATIONS = 50;
};
