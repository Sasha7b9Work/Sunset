// 2025/6/1 18:01:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "MainWindow.h"
#include "Panels/PanelMenu/PanelMenu.h"
#include "Controls/ButtonsCombo.h"
#include "Panels/PanelConfig/PanelConfig.h"
#include "Panels/PanelIndicator.h"


class PanelModeLower : public Panel
{
public:

    static const int WIDTH = MainWindow::WIDTH3;
    static const int HEIGHT = MainWindow::HEIGHT - PanelConfig::HEIGHT - PanelIndicator::HEIGHT;

    PanelModeLower(wxWindow *parent);

    static PanelModeLower *self;

private:

    wxButton *btnExit = nullptr;

    void OnEventButton(wxCommandEvent &);
};
