// 2026/04/01 14:21:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"


class PainterDataFPGA : public Painter
{
public:

    PainterDataFPGA(wxWindow *, const wxSize &);
};


class ControlDataFPGA : private wxPanel
{
public:

    const wxSize SIZE{ 185, 85 };

    ControlDataFPGA(wxWindow *, const wxPoint &);

    void SetMax(int);

    void Draw();

    void SetData(int[MAX_NUMBER_POINTS]);

private:

    PainterDataFPGA *painter = nullptr;
    ToggleButton *btnScale = nullptr;
    int max = (1 << 18) - 1;
    int data[MAX_NUMBER_POINTS];

    void OnEventToggleButon(wxCommandEvent &);
};
