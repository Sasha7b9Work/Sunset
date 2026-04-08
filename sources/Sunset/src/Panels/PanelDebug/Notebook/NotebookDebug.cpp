// 2025/6/3 14:06:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/NotebookDebug.h"
#include "Panels/PanelDebug/Notebook/PageFPGA.h"
#include "Panels/PanelDebug/Notebook/PageSource3kV.h"
#include "Panels/PanelDebug/Notebook/PageCommutator.h"
#include "Panels/PanelDebug/Notebook/PageChannelC.h"
#include "Panels/PanelDebug/Notebook/PageChannelB.h"
#include "Panels/PanelDebug/Notebook/PageChannelS.h"
#include "Panels/PanelDebug/Notebook/PageMeasCurrent.h"
#include "Panels/PanelDebug/Notebook/PageSource50V.h"


NotebookDebug *TheNotebookDebug = nullptr;


NotebookDebug::NotebookDebug(wxWindow *parent) :
    wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    TheNotebookDebug = this;

    AppendNewPage(new PageFPGA(this));
    AppendNewPage(new PageCommutator(this));
    AppendNewPage(new PageSource3kV(this));
    AppendNewPage(new PageChannelC(this));
    AppendNewPage(new PageChannelB(this));
    AppendNewPage(new PageChannelS(this));
    AppendNewPage(new PageMeasCurrent(this));
    AppendNewPage(new PageSource50V(this));

    wxWindowBase::Layout();

    Bind(wxEVT_NOTEBOOK_PAGE_CHANGING, &NotebookDebug::OnEventPageChanged, this);

    SetMaxSize({ 812, -1 });
}


void NotebookDebug::AppendNewPage(wxPanel *page)
{
    wxNotebook::AddPage(page, page->GetName());
}


void NotebookDebug::Init()
{
    for (int i = 0; i < DAC::Count; i++)
    {
        dacs[i]->WriteWidthToDevice();
    }
}


void NotebookDebug::DeInit()
{
}


void NotebookDebug::PeriodicTask()
{
}


void NotebookDebug::Pack()
{
    PageFPGA::self->Pack();
    PageCommutator::self->Pack();
    PageSource3kV::self->Pack();
    PageChannelC::self->Pack();
    PageChannelB::self->Pack();
    PageChannelS::self->Pack();
    PageMeasCurrent::self->Pack();
    PageSource50V::self->Pack();

    Config::WriteInt("pagedebug_page", GetSelection());
}


void NotebookDebug::Unpack()
{
    PageFPGA::self->Unpack();
    PageCommutator::self->Unpack();
    PageSource3kV::self->Unpack();
    PageChannelC::self->Unpack();
    PageChannelB::self->Unpack();
    PageChannelS::self->Unpack();
    PageMeasCurrent::self->Unpack();
    PageSource50V::self->Unpack();

    SetSelection((size_t)Config::ReadInt("pagedebug_page", 0));
}


void NotebookDebug::OnEventPageChanged(wxBookCtrlEvent &event)
{
    if (switching_allowed)
    {
        Pack();
    }
    else
    {
        event.Veto();
    }
}


void NotebookDebug::EnableSwitching(bool enable)
{
    static int num_blocksed = 0;        // Количество блокирующих воздействий.
                                        // Переключение страниц возможно только когда это значение равно нулю

    if (enable)
    {
        num_blocksed--;
    }
    else
    {
        num_blocksed++;
    }

    switching_allowed = (num_blocksed == 0);
}