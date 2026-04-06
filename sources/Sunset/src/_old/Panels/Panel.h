// 2025/6/1 16:29:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Panel : public wxPanel
{
public:
    Panel(wxWindow *parent, int x, int y, int w, int h) :
        wxPanel(parent, wxID_ANY, { x, y }, { w, h }, wxTAB_TRAVERSAL | wxSUNKEN_BORDER)
    {
    }
};
