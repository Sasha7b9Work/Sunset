// 2025/6/3 13:44:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PanelDebug.h"
#include "MainWindow.h"
#include "Panels/PanelDebug/Notebook/NotebookDebug.h"
#include "Panels/PanelDebug/PanelRight.h"
#pragma warning(push, 0)
#include <wx/sizer.h>
#pragma warning(pop)


PanelDebug *PanelDebug::self = nullptr;


PanelDebug::PanelDebug(Notebook *board) : PageNotebook(board, L("Отладка"))
{
    self = this;

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(new NotebookDebug(this));

    sizer->Add(new PanelRight(this, ThePanelRight));

    SetSizer(sizer);

    PageNotebook::Layout();

    Unpack();
}


bool PanelDebug::Show(bool show)
{
    show ? NotebookDebug::self->Init() : NotebookDebug::self->DeInit();

    return wxPanel::Show(show);
}


void PanelDebug::PeriodicTask()
{
    NotebookDebug::self->PeriodicTask();

//    ThePanelRight->PeriodicTask();
}


void PanelDebug::Unpack()
{
    NotebookDebug::self->Unpack();
}


void PanelDebug::Pack()
{
    NotebookDebug::self->Pack();
}
