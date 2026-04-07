// 2025/08/14 11:24:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelChannelBS.h"
#include "Utils/SystemDepend.h"
#include "MainWindow.h"
#include "Panels/PanelConfig/PanelConfig.h"
#include "IPPP/Tests/Ranges.h"
#include "Utils/StringUtils.h"
#include "Utils/GlobalFunctions.h"
#include "IPPP/Tests/Tests.h"
#include "Controls/StaticBox.h"


PanelChannelB *ThePanelChannelB = nullptr;
PanelChannelS *ThePanelChannelS = nullptr;


PanelChannelBS::PanelChannelBS(wxPanel *parent, Chan::E ch, int x, int w, int h) :
    wxPanel(parent)
{
    wxPanel::SetName(wxString("PanelChannel") + ((ch == Chan::_B) ? "B" : "S"));

    SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - PanelConfig::HEIGHT_BUTTONS });
    SetPosition({ 0, PanelConfig::HEIGHT_BUTTONS });

    StaticBox *boxGenerator = new StaticBox(this, "Генератор ступенек", { x, SD::DSBY() }, { w, 320 });

    {
        int y = 22;

        wxArrayString choices;
        choices.Add("U");
        choices.Add("I");

        comboTypeGenerator = new ButtonsCombo(boxGenerator, "Тип", SD::XY0(), PanelConfig::WIDTH_COMBO, choices, choices, 1, "comboTypeGenerator");

        y = SD::XY0().y;

        y += PanelConfig::DYC;

        choices.Clear();
        choices.Add("Вкл");
        choices.Add("Выкл");

        comboImpulse = new ButtonsCombo(boxGenerator, "Импульс", { SD::XY0().x, y }, PanelConfig::WIDTH_COMBO, choices, choices, 1, "comboImpulse");

        y += PanelConfig::DYC;

        choices.Clear();
        choices.Add("-");

        comboStep = new ButtonsComboRange(boxGenerator, "Амплитуда ступени", { SD::XY0().x, y }, PanelConfig::WIDTH_COMBO, choices, choices, "comboStep");

        y += PanelConfig::DYC;

        choices = { "x 1", "x 0.1" };

        comboAmpitudeDecrease = new ButtonsCombo(boxGenerator, "Амплитуда", { SD::XY0().x, y }, PanelConfig::WIDTH_COMBO, choices, choices, 1, "comboAmpitudeDecrease");

        y += PanelConfig::DYC;

        choices.Clear();

        for (int i = 0; i <= 10; i++)
        {
            choices.push_back(wxString::Format("%d", i));
        }

        comboNumberSteps = new ButtonsCombo(boxGenerator, wxT("Число ступенек"), { SD::XY0().x, y }, PanelConfig::WIDTH_COMBO, choices, choices, 3, "comboNumberSteps");

        y += PanelConfig::DYC;

        choices.Clear();
        choices.Add(wxT("прямая"));
        choices.Add(wxT("обратная"));

        comboPolarity = new ButtonsCombo(boxGenerator, wxT("Полярность"), { SD::XY0().x, y }, PanelConfig::WIDTH_COMBO, choices, choices, 1, "comboPolarity");

        y += 30;

        StaticBox *boxOffset = new StaticBox(boxGenerator, wxT("Смещение"), { x, y }, { w - 10, 100 });

        {
            y = 25;

            sliderOffset = new SliderFloatOffset(boxOffset, { SD::XY0().x, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO - 10, "sliderOffset");

            y += 40;

            choices.Clear();
            choices.Add(wxT("прямая"));
            choices.Add(wxT("обратная"));

            comboOffsetPolarity = new ButtonsCombo(boxOffset, wxT("Полярность"), { PanelConfig::X, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO - 10, choices, choices, 1, "comboOffsetPolarity");
        }

        boxOffset->SetFont(StaticBox::TitleFont());

        wxPoint pos = boxOffset->GetPosition();
        pos.y = SD::Y_SB(boxGenerator->GetSize().y - boxOffset->GetSize().y - 28); //-V807
        boxOffset->SetPosition(pos);
    }

    boxGenerator->SetFont(StaticBox::TitleFont());

    StaticBox *boxLimitation = new StaticBox(this, wxT("Ограничение"),
        { x, boxGenerator->GetPosition().y + boxGenerator->GetSize().y + SD::DSBY() },
        { w, h - boxGenerator->GetPosition().y - boxGenerator->GetSize().y - SD::DSBY() });

    {
        int y = 30;

        wxArrayString choices;
        choices.Add("-");

        comboLimitRange = new ButtonsComboRange(boxLimitation, "Диапазон", SD::XY0(), PanelConfig::WIDTH_COMBO, choices, choices, "comboLimitRange");

        y += 40;

        sliderLimit = new SliderFloatLimit(boxLimitation, { PanelConfig::X, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO, "sliderLimit");
    }

    boxLimitation->SetFont(StaticBox::TitleFont());

    Bind(wxEVT_COMBOBOX, &PanelChannelB::OnEventComboBox, this);

    Tune();
}


void PanelChannelBS::Tune()
{
    comboTypeGenerator->SetCurrentSelection(1);

    wxArrayString ranges;
    RangeI::FillArrayStrings(ranges, TypeValue::Limit_B, false);
    comboLimitRange->SetChoices(ranges, ranges);

    {
        comboStep->SetLastSelection();

        GF::SendCommandEvent(comboStep, wxEVT_COMBOBOX, comboStep->GetCurrentSelection());
    }

    {
        comboLimitRange->SetLastSelection();

        GF::SendCommandEvent(comboLimitRange, wxEVT_COMBOBOX, comboLimitRange->GetCurrentSelection());
    }
}


void PanelChannelBS::OnEventComboBox(wxCommandEvent &event)
{
    ButtonsCombo *combo = (ButtonsCombo *)event.GetEventObject();

    if (combo == comboTypeGenerator)
    {
        wxArrayString ranges;

        if (combo->GetCurrentSelection() == 0)              // Напряжение
        {
            RangeU::FillArrayStrings(ranges, TypeValue::Source_B, true);

            wxArrayString tooltips;
            RangeU::FillArrayStrings(tooltips, TypeValue::Source_B, false);

            for (auto &elem : tooltips)
            {
                elem = wxString(wxT("Диапазон : ") + elem);
            }

            comboStep->SetChoices(ranges, tooltips);
        }
        else if (combo->GetCurrentSelection() == 1)         // Ток
        {
            RangeI::FillArrayStrings(ranges, TypeValue::Source_B, true);

            wxArrayString tooltips;
            RangeI::FillArrayStrings(tooltips, TypeValue::Source_B, false);

            for (auto &elem : tooltips)
            {
                elem = wxString(wxT("Диапазон : ")) + elem;
            }

            comboStep->SetChoices(ranges, tooltips);
        }
    }
    else if (combo == comboLimitRange)
    {
        sliderLimit->CalculateAndSetRange(comboLimitRange->GetCurrentString());
    }

    if (comboTypeGenerator->GetCurrentSelection() == 0)             // Напряжение
    {
        sliderOffset->CalculateAndSetRange(comboStep->GetCurrentString(), 10.0);
    }
    else if (comboTypeGenerator->GetCurrentSelection() == 1)        // Ток
    {
        sliderOffset->CalculateAndSetRange(comboStep->GetCurrentString(), 10.0);
    }

    event.Skip();
}


bool PanelChannelB::IsEnabled() const
{
    return Chan(Chan::_B).IsVisible();
}


bool PanelChannelS::IsEnabled() const
{
    return Chan(Chan::_S).IsVisible();
}


void PanelChannelBS::Pack()
{
    comboTypeGenerator->Pack();
    comboImpulse->Pack();
    comboStep->Pack();
    comboAmpitudeDecrease->Pack();
    comboNumberSteps->Pack();
    comboPolarity->Pack();
    comboOffsetPolarity->Pack();
    comboLimitRange->Pack();

    sliderLimit->Pack();
    sliderOffset->Pack();
}


void PanelChannelBS::Unpack()
{
    comboTypeGenerator->Unpack();
    wxYield();
    comboImpulse->Unpack();
    comboStep->Unpack();
    comboAmpitudeDecrease->Unpack();
    comboNumberSteps->Unpack();
    comboPolarity->Unpack();
    comboOffsetPolarity->Unpack();
    comboLimitRange->Unpack();

    sliderLimit->Unpack();
    sliderOffset->Unpack();
}
