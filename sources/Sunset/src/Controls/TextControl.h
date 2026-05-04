// 2023/08/09 13:34:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#pragma warning(push, 0)
#include <wx/textctrl.h>
#pragma warning(pop)


class TextCtrlNumber : public wxTextCtrl
{
public:

    TextCtrlNumber(wxWindow *, int, const wxString &, const wxSize &, int min, int max);

private:

    void OnEventChar(wxKeyEvent &);

    int m_min;
    int m_max;
};
