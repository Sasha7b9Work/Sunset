// 2025/8/9 10:39:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/BmpButtonsCombo.h"
#include "Controls/ButtonsCombo.h"
#include "Controls/Slider.h"
#include "Controls/Buttons.h"


class PanelChannelC : public wxPanel
{
    friend class PanelScheme;

public:

    PanelChannelC(wxPanel *parent, int x, int w, int h);

    void Pack();
    void Unpack();

private:

    ButtonsComboRange *comboMeasVoltage = nullptr;          // Измеритель U
    ButtonsComboRange *comboMeasCurrent = nullptr;          // Измеритель I
    ButtonsCombo      *comboSourceRange = nullptr;          // Источник U - Диапазон
    wxStaticText      *txtLimit = nullptr;                  // Источник U - Ограничение %
    BmpButtonsCombo   *comboScan = nullptr;                 // Развёртка
    ButtonsCombo      *comboDutyCycleIncrease = nullptr;    // Развёртка - Скважность
    ButtonsCombo      *comboNumberPoints = nullptr;         // Развёртка - Число точек
    wxStaticText      *textLabelImpulse = nullptr;          // Развёртка - Длина импульса
    wxStaticText      *textValueImpulse = nullptr;          // Развёртка - Длина импульса (значение)
    SliderInt         *spinStart = nullptr;                 // Измеритель - Ограничение - Старт
    SliderInt         *spinStop = nullptr;                  // Измеритель - Ограничение - Стоп

    void Tune();

    void OnEventComboBox(wxCommandEvent &);
};