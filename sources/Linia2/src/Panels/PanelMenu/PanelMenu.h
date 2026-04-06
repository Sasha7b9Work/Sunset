// 2025/6/1 17:31:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "Panels/WindowCalculation/WindowCalculation.h"


class PanelMenu : public Panel
{
public:

    static const int HEIGHT = 35;

    PanelMenu(wxWindow *parent);

    static PanelMenu *self;

private:

    wxButton *btnModels = nullptr,
        *btnTests = nullptr,
        *btnSetup = nullptr,
        *btnArchiv = nullptr,
        *btnTable = nullptr,
        *btnReport = nullptr,
        *btnDebug = nullptr;

    WindowCalculation *wndCalculation = nullptr;

    void OnEventButton(wxCommandEvent &);

    int NumButtons() const;
};
