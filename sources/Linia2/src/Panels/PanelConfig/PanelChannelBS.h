// 2025/08/14 11:24:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ButtonsCombo.h"
#include "Controls/SpinBox.h"
#include "Controls/Slider.h"
#include "Controls/Buttons.h"
#include "IPPP/Tests/SettingsTests.h"


class PanelChannelBS : public wxPanel
{
    friend class PanelChannelB;
    friend class PanelChannelS;
    friend class PanelScheme;

public:

    void Pack();
    void Unpack();

private:

    PanelChannelBS(wxPanel *parent, Chan::E ch, int x, int w, int h);

    ButtonsCombo      *comboTypeGenerator = nullptr;    // Тип генерации - напряжение или ток
    ButtonsCombo      *comboImpulse = nullptr;          // Импульс
    ButtonsComboRange *comboStep = nullptr;             // Амплитуда ступени
    ButtonsComboRange *comboLimitRange = nullptr;       // Ограничение - Диапазон
    SliderFloatOffset *sliderOffset = nullptr;          // Смещение
    SliderFloatLimit  *sliderLimit = nullptr;           // Ограничение
    ButtonsCombo      *comboAmpitudeDecrease = nullptr; // Амплитуда / 10
    ButtonsCombo      *comboNumberSteps = nullptr;      // Число ступенек
    ButtonsCombo      *comboPolarity = nullptr;         // Полярность
    ButtonsCombo      *comboOffsetPolarity = nullptr;   // Смещение - Полярность

    void Tune();

    void OnEventComboBox(wxCommandEvent &);
};


class PanelChannelB : public PanelChannelBS
{
    friend class PanelScheme;

public:

    PanelChannelB(wxPanel *parent, int x, int w, int h) :
        PanelChannelBS(parent, Chan::_B, x, w, h)
    {
        ThePanelChannelB = this;
    }

    bool IsEnabled() const;
};


class PanelChannelS : public PanelChannelBS
{
    friend class PanelScheme;

public:

    PanelChannelS(wxPanel *parent, int x, int w, int h) :
        PanelChannelBS(parent, Chan::_S, x, w, h)
    {
        ThePanelChannelS = this;
    }

    bool IsEnabled() const;
};

