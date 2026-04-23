// 2025/7/23 14:18:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Panel.h"


/*
*   Собственный спинбокс. Разработанн ввиду того, что стандартный на Linux очень большой
*/


class SpinBox : public Panel
{
public:

    SpinBox(wxWindow *parent, const wxSize &, int min, int max);

    int GetValue() const;

    void SetValue(int);

private:

    int min = 0;
    int max = 0;

    wxTextCtrl *text = nullptr;
    wxButton *btnMore = nullptr;
    wxButton *btnLess = nullptr;

    void OnEventButton(wxCommandEvent &);
};
