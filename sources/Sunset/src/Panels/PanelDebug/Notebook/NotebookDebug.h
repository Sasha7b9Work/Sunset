// 2025/6/3 14:06:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/notebook.h>
#pragma warning(pop)


class wxPanel;


class NotebookDebug : public wxNotebook
{
public:

    NotebookDebug(wxWindow *, NotebookDebug *&);

    void Init();

    void PeriodicTask();

    void DeInit();

    void Unpack();

    void Pack();

    void EnableSwitching(bool);

private:

    bool switching_allowed = true;

    void AppendNewPage(wxPanel *);

    void OnEventPageChanged(wxBookCtrlEvent &);

    void OnEventSetCursor(wxSetCursorEvent &);
};
