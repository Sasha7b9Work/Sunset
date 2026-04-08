// 2026/04/07 09:07:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panels.h"


Panel::Panel(wxWindow *parent, const wxString &_name) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxSUNKEN_BORDER),
    name(_name)
{
    Bind(wxEVT_SIZE, &Panel::OnEventSize, this);
}


const wxString &Panel::GetPanelName() const
{
    return name;
}


void Panel::OnEventSize(wxSizeEvent &event)
{
    CallbackOnEventSize();

    Refresh();

    event.Skip();
}
