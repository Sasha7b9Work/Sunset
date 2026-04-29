// 2026/04/29 11:33:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/panel.h>
#pragma warning(pop)


class PanelCategory : public wxPanel
{
public:

    PanelCategory(wxWindow *, PanelCategory *&);
};
