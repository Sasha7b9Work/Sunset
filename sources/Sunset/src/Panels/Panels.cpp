// 2026/04/07 09:07:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panels.h"


PanelBoard::PanelBoard(wxWindow *parent) :
    wxPanel(parent)
{

}


Panel::Panel(wxWindow *parent, int x, int y, int w, int h) :
    wxPanel(parent, wxID_ANY, { x, y }, { w, h }, wxTAB_TRAVERSAL | wxSUNKEN_BORDER)
{
}
