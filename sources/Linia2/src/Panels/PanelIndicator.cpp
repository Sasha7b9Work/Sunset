// 2025/6/1 18:25:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelConfig.h"
#include "Panels/PanelIndicator.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "Controls/Bitmap.h"
#include "Panels/PanelConfig/PanelConfig.h"
#include "IPPP/I_IPPP.h"


PanelIndicator *PanelIndicator::self = nullptr;


PanelIndicator::PanelIndicator(wxWindow* parent) :
    Panel(parent, MainWindow::WIDTH_DRAW, PanelConfig::HEIGHT,
        MainWindow::WIDTH3, HEIGHT)
{
    Panel::SetName("PanelIndicator");

    self = this;

    wxArrayString images =
    {
        "buttons/start_off.bmp",
        "buttons/start_on.bmp"
    };

    button_start = new ButtonBitmapChoice(this, {0, 0}, {64 - 10, 60 - 10}, images, "button_start");

    wxStaticText *text = new wxStaticText(this, wxID_ANY, "Uc 100 %", { 90, 15 }, { 25, 25});

    text->SetForegroundColour(wxColour(255, 0, 0));

    wxFont font = text->GetFont();
    font.SetPointSize(font.GetPointSize() + 7);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    text->SetFont(font);

    button_start->Bind(EVT_BUTTON_BITMAP_CHOICE, &PanelIndicator::OnEventButton, this);
} //-V773


void PanelIndicator::OnEventButton(ButtonBitmapChoiceEvent &event)
{
    int id = event.GetId();
    int value = event.GetInt();

    if (id == button_start->GetId())
    {
        if (value == 0)
        {
            ThePanelConfig->EnableSubPanels(true);
            I_IPPP::GetInstance()->PressButtonStop();
        }
        else if (value == 1)
        {
            ThePanelConfig->EnableSubPanels(false);
            I_IPPP::GetInstance()->PressButtonStart();
        }
    }

    event.Skip();
}
