// 2025/6/1 18:03:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelModel.h"
#include "Panels/PanelModeLower.h"
#include "Utils/SystemDepend.h"
#include "Controls/StaticBox.h"
#include "Panels/PanelConfig/PanelConfig.h"
#include "Panels/PanelIndicator.h"
#include "MainWindow.h"


PanelModeLower *PanelModeLower::self = nullptr;


PanelModeLower::PanelModeLower(wxWindow *parent) :
    Panel(parent, MainWindow::WIDTH_DRAW, PanelConfig::HEIGHT + PanelIndicator::HEIGHT, WIDTH, HEIGHT)
{
    Panel::SetName("PanelModeLower");

    self = this;

    StaticBox *boxCategory = new StaticBox(this, wxT("Режим"), { 5, 0 }, { WIDTH - 13, HEIGHT - 8});

    {
        new wxStaticText(boxCategory, wxID_ANY, wxT("Канал C:"), { 10, SD::Y_SB(30) });

        new wxStaticText(boxCategory, wxID_ANY, wxT("Канал B:"), { 10, SD::Y_SB(60) });

        wxArrayString choices;
        choices.Add("C");

        new ButtonsCombo(boxCategory, wxT("1 очередь"), { 5, 120 }, 100, choices, choices, 1, "comboFirst");

        btnExit = new wxButton(boxCategory, wxID_ANY, "Х", { WIDTH - 35, HEIGHT - 52 }, { 20, 20 });
    }

    boxCategory->SetFont(boxCategory->TitleFont());

    Bind(wxEVT_BUTTON, &PanelModeLower::OnEventButton, this);
}


void PanelModeLower::OnEventButton(wxCommandEvent &event)
{
    if (event.GetId() == btnExit->GetId())
    {
        TheMainWindow->Close();
    }

    event.Skip();
}
