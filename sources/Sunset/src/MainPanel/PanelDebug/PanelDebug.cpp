// 2025/6/3 13:44:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainPanel/PanelDebug/PanelDebug.h"
#include "MainWindow.h"
#include "MainPanel/PanelDebug/Notebook/NotebookDebug.h"
#include "MainPanel/PanelDebug/PanelRight.h"
#pragma warning(push, 0)
#include <wx/sizer.h>
#pragma warning(pop)


PanelDebug *ThePanelDebug = nullptr;


PanelDebug::PanelDebug(Notebook *board, PanelDebug *&self) : PageNotebook(board, L("Отладка"))
{
    self = this;

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(new NotebookDebug(this, TheNotebookDebug));

    sizer->Add(new PanelRight(this, ThePanelRight));

    SetSizer(sizer);

    PageNotebook::Layout();

    Unpack();
}


bool PanelDebug::Show(bool show)
{
    show ? TheNotebookDebug->Init() : TheNotebookDebug->DeInit();

    return wxPanel::Show(show);
}


void PanelDebug::PeriodicTask()
{
    TheNotebookDebug->PeriodicTask();

//    ThePanelRight->PeriodicTask();
}


void PanelDebug::Unpack()
{
    TheNotebookDebug->Unpack();
}


void PanelDebug::Pack()
{
    TheNotebookDebug->Pack();
}
