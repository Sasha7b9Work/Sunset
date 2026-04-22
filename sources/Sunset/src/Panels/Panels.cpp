// 2026/04/07 09:07:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panels.h"
#include "Panels/PanelBoard.h"


Panel::Panel(Notebook *board, const wxString &_name) :
    wxPanel(board->GetCenterContainer(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxSUNKEN_BORDER),
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
    const wxSize size = GetParent()->GetSize();

    if (size.x && size.y)
    {
        if (size != prev_size)
        {
            prev_size = size;

            SetMinSize(size);
            SetSize(size);

            Refresh();

            CallbackOnEventSize();
            Refresh();
        }
    }

    event.Skip();
}
