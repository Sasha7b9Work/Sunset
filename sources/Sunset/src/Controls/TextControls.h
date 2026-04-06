// 2023/08/09 13:34:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"


class TextCtrlNumber : public wxTextCtrl
{
public:

    TextCtrlNumber(wxWindow *, int, const wxString &, const wxPoint &, const wxSize &, int min, int max);

private:

    void OnEventChar(wxKeyEvent &);

    int min;
    int max;
};
