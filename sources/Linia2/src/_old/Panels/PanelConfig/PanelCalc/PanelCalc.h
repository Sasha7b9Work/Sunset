// 2025/6/1 17:41:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "MainWindow.h"
#include "Panels/PanelMenu/PanelMenu.h"
#include "Panels/PanelConfig/PanelCalc/WindowCursors.h"


class PanelCalc : public wxPanel
{
    friend class PanelConfig;

public:

    static const int HEIGHT = MainWindow::HEIGHT - MainWindow::HEIGHT_HI - PanelMenu::HEIGHT;

private:

    wxButton      *btnCursors = nullptr;

    PanelCalc(wxWindow *parent, int x, int w, int h);

    void OnEventButton(wxCommandEvent &);
};
