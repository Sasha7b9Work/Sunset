// 2025/7/27 19:54:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"
#include "Controls/ButtonsCombo.h"


// Настройка - Калибровка


class WindowCalibrate : public Dialog
{
public:

    static const int WIDTH = 450;
    static const int HEIGHT = 650;

    WindowCalibrate();

private:

    ButtonsCombo *comboRange = nullptr;

    wxStaticText *textSerialNumber = nullptr,
        *textDateTime = nullptr;

    wxRadioButton *rbChanC_MeasI = nullptr,
        *rbChanC_MeasU = nullptr,
        *rbChanB_MeasI = nullptr,
        *rbChanB_MeasU = nullptr,
        *rbChanS_MeasI = nullptr,
        *rbChanS_MeasU = nullptr;

    void OnEventButton(wxCommandEvent &);
};
