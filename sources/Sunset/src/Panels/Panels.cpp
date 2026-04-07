// 2026/04/07 09:07:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panels.h"


PanelBoard::PanelBoard(wxWindow *parent) :
    wxPanel(parent)
{

}


void PanelBoard::AddPanel(Panel *)
{

}


Panel::Panel(wxWindow *parent) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxSUNKEN_BORDER)
{
}
