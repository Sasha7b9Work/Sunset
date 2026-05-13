// 2026/04/23 08:57:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Panel.h"
#pragma warning(push, 0)
#include <wx/sizer.h>
#pragma warning(pop)


Panel::Panel(wxWindow *parent, long style) : //-V126
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, style)
{
}


ScrolledPanel::ScrolledPanel(wxWindow *parent) :
    wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER)
{
    SetScrollRate(5, 5);
}


void ScrolledPanel::SetSizer(wxSizer *sizer, bool delete_old)
{
    wxScrolledWindow::SetSizer(sizer, delete_old);

    wxScrolledWindow::FitInside();

    wxScrolledWindow::SetVirtualSize(sizer->GetMinSize());
}
