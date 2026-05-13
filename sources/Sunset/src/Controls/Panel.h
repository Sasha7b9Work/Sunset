// 2026/04/23 08:55:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/scrolwin.h>
#include <wx/panel.h>
#pragma warning(pop)


class Panel : public wxPanel
{
public:

    Panel(wxWindow *, long style = 0); //-V126
};


class ScrolledPanel : public wxScrolledWindow
{
public:

    ScrolledPanel(wxWindow *);

    void SetSizer(wxSizer *, bool delete_old = true);
};
