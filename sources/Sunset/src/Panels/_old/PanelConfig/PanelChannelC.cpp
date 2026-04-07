// 2025/8/9 10:40:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelChannelC.h"
#include "Utils/SystemDepend.h"
#include "Controls/SpinBox.h"
#include "MainWindow.h"
#include "Panels/PanelConfig/PanelConfig.h"
#include "IPPP/Tests/Ranges.h"
#include "Controls/StaticBox.h"


PanelChannelC *ThePanelChannelC = nullptr;


PanelChannelC::PanelChannelC(wxPanel *parent, int x, int w, int h) :
    wxPanel(parent)
{
    wxPanel::SetName(wxT("PanelChannelC"));

    ThePanelChannelC = this;

    wxPanel::SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - PanelConfig::HEIGHT_BUTTONS });
    wxPanel::SetPosition({ 0, PanelConfig::HEIGHT_BUTTONS });

    StaticBox *boxScan = new StaticBox(this, _("Развёртка"), { x, SD::DSBY() }, { w, 160 });

    {
        wxArrayString files;
        wxArrayString tooltips;

        for (uint i = 0; i < TypeScan::Count; i++)
        {
            files.push_back(TypeScan::NameFileICO((TypeScan::E)i));
            tooltips.push_back(TypeScan::_NameGUI((TypeScan::E)i));
        }

        comboScan = new BmpButtonsCombo(boxScan, "Развёртка", { 18, SD::XY0().y }, { 32, 32 }, files, tooltips, 0, 3, "comboScan");

        int y = 25;
        int x0 = 80;

        textLabelImpulse = new wxStaticText(boxScan, wxID_ANY, "Длина импульса", { x0, SD::Y_SB(y) });
        textValueImpulse = new wxStaticText(boxScan, wxID_ANY, "0.2 ms", { x0 + 30, SD::Y_SB(y + 15) });

        y = 80;

        wxArrayString names = { "20", "50" };

        comboNumberPoints = new ButtonsCombo(boxScan, "Число точек", { PanelConfig::X, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO, names, names, 1, "comboNumberPoints");

        y += PanelConfig::DYC;

        names = { "x 1", "x 2" };

        comboDutyCycleIncrease = new ButtonsCombo(boxScan, "Скважность", { PanelConfig::X, SD::Y_SB(y) }, PanelConfig::WIDTH_COMBO, names, names, 1, "comboDutyCycleIncrease");
    }

    boxScan->SetFont(StaticBox::TitleFont());

    StaticBox *boxMeter = new StaticBox(this, "Измеритель", { x, boxScan->GetPosition().y + boxScan->GetSize().y + SD::DSBY() }, { w, 70 });

    {
        wxArrayString names;
        int width = 85;

        RangeU::FillArrayStrings(names, TypeValue::Meas_C, false);
        comboMeasVoltage = new ButtonsComboRange(boxMeter, "Uc", SD::XY0(), width, names, names, "comboVoltage");

        RangeI::FillArrayStrings(names, TypeValue::Meas_C, false);
        comboMeasCurrent = new ButtonsComboRange(boxMeter, "Ic", { SD::XY0().x + PanelConfig::WIDTH_COMBO - width, SD::XY0().y }, width, names, names, "comboCurrent");
    }

    boxMeter->SetFont(StaticBox::TitleFont());

    StaticBox *boxSource = new StaticBox(this, "Источник U", { x, boxMeter->GetPosition().y + boxMeter->GetSize().y + SD::DSBY() }, { w, h - boxMeter->GetPosition().y - boxMeter->GetSize().y - SD::DSBY() });

    {
        wxArrayString names
        {
            "5 V",
            "20 V",
            "100 V",
            "500 V",
            "2 kV"
        };

        comboSourceRange = new ButtonsCombo(boxSource, "Диапазон Ud", SD::XY0(), PanelConfig::WIDTH_COMBO, names, names, 1, "comboRange");

        txtLimit = new wxStaticText(boxSource, wxID_ANY, "Ограничение Uc, %", { 40, SD::Y_SB(65) });

        int y = 90;

        new wxStaticText(boxSource, wxID_ANY, "Старт", { 10, SD::Y_SB(y) });

        y += 20;

        int width = 180;
        spinStart = new SliderInt(boxSource, { 10, SD::Y_SB(y) }, width, 0, 100, "spinStart");

        y += 40;

        new wxStaticText(boxSource, wxID_ANY, "Стоп", { 10, SD::Y_SB(y) });

        y += 20;

        spinStop = new SliderInt(boxSource, { 10, SD::Y_SB(y) }, width, 0, 100, "spinStop");
    }

    boxSource->SetFont(StaticBox::TitleFont());

    Bind(wxEVT_COMBOBOX, &PanelChannelC::OnEventComboBox, this);

    Tune();
}


void PanelChannelC::Tune()
{
    comboScan->SetCurrentChoice(1);
}


void PanelChannelC::OnEventComboBox(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == comboSourceRange->GetId())
    {
        static const pchar values[5] =
        {
            "0.2 ms",
            "0.2 ms",
            "0.5 ms",
            "1.0 ms",
            "2.0 ms"
        };

        textValueImpulse->SetLabel(values[comboSourceRange->GetCurrentSelection()]);
    }
    else if (id == comboScan->GetId())
    {
        int scan = comboScan->GetCurrentChoice();

        bool show = scan < 2;

        textLabelImpulse->Show(show);
        textValueImpulse->Show(show);

        static const bool duty[7] =
        {
            true, true, false, false, true, true, true
        };

        comboDutyCycleIncrease->Show(duty[scan]);

        {
            wxArrayString choices;

            if (scan < 4)
            {
                choices.push_back("20");
                choices.push_back("50");
            }
            else
            {
                choices.push_back("200");
            }

            comboNumberPoints->SetChoices(choices, choices);
        }
    }
}


void PanelChannelC::Pack()
{
    comboScan->Pack();
    comboNumberPoints->Pack();
    comboDutyCycleIncrease->Pack();
    comboMeasVoltage->Pack();
    comboMeasCurrent->Pack();
    spinStart->Pack();
    spinStop->Pack();
    comboSourceRange->Pack();
}


void PanelChannelC::Unpack()
{
    comboScan->Unpack();
    wxYield();                      // Немедленно обрабатываем все события
    comboNumberPoints->Unpack();
    comboDutyCycleIncrease->Unpack();
    comboMeasVoltage->Unpack();
    comboMeasCurrent->Unpack();
    spinStart->Unpack();
    spinStop->Unpack();
    comboSourceRange->Unpack();
}
