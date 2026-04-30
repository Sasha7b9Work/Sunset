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
#include "IPPP/Real/Chips.h"
#include "Settings/Configurator.h"


NotebookDebug *TheNotebookDebug = nullptr;


NotebookDebug::NotebookDebug(wxWindow *parent, NotebookDebug *&self) :
    wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    self = this;

    AppendNewPage(new PageFPGA(this));
    AppendNewPage(new PageChannelC(this));
    AppendNewPage(new PageMeasCurrent(this));
    AppendNewPage(new PageChannelB(this));
    AppendNewPage(new PageChannelS(this));
    AppendNewPage(new PageSource3kV(this));
    AppendNewPage(new PageSource50V(this));
    AppendNewPage(new PageCommutator(this));

    wxWindowBase::Layout();

    Bind(wxEVT_NOTEBOOK_PAGE_CHANGING, &NotebookDebug::OnEventPageChanged, this);

    Bind(wxEVT_SET_CURSOR, &NotebookDebug::OnEventSetCursor, this);

    SetMaxSize({ 812, -1 });
}


void NotebookDebug::OnEventSetCursor(wxSetCursorEvent &event)
{
    // Получаем позицию мыши в клиентских координатах
    wxPoint mousePos = event.GetX() > -1 ? wxPoint(event.GetX(), event.GetY()) : ScreenToClient(wxGetMousePosition());

    // Определяем, над какой вкладкой находится мышь
    int tabIdx = HitTest(mousePos);

    // Если мышь над областью вкладок, а не над клиентской областью
    if (tabIdx != wxNOT_FOUND)
    {
        // Устанавливаем курсор-руку
        event.SetCursor(wxCursor(wxCURSOR_HAND));
    }
    else
    {
        // Если мышь не над вкладкой, передаем событие дальше,
        // чтобы система могла установить курсор по умолчанию.
        event.Skip();
    }
}


void NotebookDebug::AppendNewPage(wxPanel *page)
{
    wxString name = page->GetName();
    if (name.IsEmpty())
    {
        name = wxString::Format("Page_%d", GetPageCount());
    }

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