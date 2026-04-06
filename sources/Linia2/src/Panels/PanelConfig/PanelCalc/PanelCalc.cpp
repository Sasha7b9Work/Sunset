// 2025/6/1 17:41:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelConfig/PanelCalc/PanelCalc.h"
#include "Utils/GlobalFunctions.h"
#include "IPPP/Tests/Model.h"
#include "Panels/PanelConfig/PanelConfig.h"
#include "Panels/WindowCalculation/WindowCalculation.h"
#include "Utils/SystemDepend.h"
#include "Controls/StaticBox.h"


PanelCalc *ThePanelCalc = nullptr;


PanelCalc::PanelCalc(wxWindow* parent, int x, int w, int /*_h*/) :
    wxPanel(parent)
{
    ThePanelCalc = this;

    wxPanel::SetName("PanelCalc");

    wxPanel::SetSize({ MainWindow::WIDTH3, PanelConfig::HEIGHT - PanelConfig::HEIGHT_BUTTONS });
    wxPanel::SetPosition({ 0, PanelConfig::HEIGHT_BUTTONS });

    StaticBox *box = new StaticBox(this, "", { x, SD::DSBY() }, { w, 320 });

    {
        btnCursors = new wxButton(box, wxID_ANY, wxT("Курсоры"), SD::XY0(), { 100, BUTTON_HEIGHT });
    }

    Bind(wxEVT_BUTTON, &PanelCalc::OnEventButton, this);

    TheWindowCursors = new WindowCursors();
}


void PanelCalc::OnEventButton(wxCommandEvent &event)
{
    wxObject *obj = event.GetEventObject();

    if (obj == btnCursors)
    {
        TheWindowCursors->ShowModal();
    }

    event.Skip();
}
