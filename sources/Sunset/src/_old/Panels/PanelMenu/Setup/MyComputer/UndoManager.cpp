#include "defines.h"
#include "UndoManager.h"


UndoManager &UndoManager::GetInstance()
{
    static UndoManager instance;
    return instance;
}

void UndoManager::AddOperation(const FileOperation &operation)
{
    // Удаляем все операции после текущей позиции
    if (m_currentIndex < m_operations.size())
    {
        m_operations.erase(m_operations.begin() + (int64)m_currentIndex, m_operations.end());
    }

    m_operations.push_back(operation);
    m_currentIndex++;

    // Ограничиваем размер истории
    if (m_operations.size() > MAX_OPERATIONS)
    {
        m_operations.erase(m_operations.begin());
        m_currentIndex--;
    }
}

FileOperation UndoManager::Undo()
{
    if (!CanUndo())
    {
        throw std::runtime_error("Nothing to undo");
    }
    m_currentIndex--;
    return m_operations[m_currentIndex];
}

FileOperation UndoManager::Redo()
{
    if (!CanRedo())
    {
        throw std::runtime_error("Nothing to redo");
    }
    FileOperation op = m_operations[m_currentIndex];
    m_currentIndex++;
    return op;
}

void UndoManager::Clear()
{
    m_operations.clear();
    m_currentIndex = 0;
}
