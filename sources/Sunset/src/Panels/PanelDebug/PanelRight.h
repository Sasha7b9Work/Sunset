// 2026/01/24 21:41:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ControlDataFPGA.h"
#include "Controls/Button.h"
#include "Controls/Panel.h"


class wxTextCtrl;


class PanelRight : public Panel
{
public:

    PanelRight(wxWindow *, PanelRight *&);

    ControlDataFPGA *data[5];           // Отображение данных, считанных c платы контроллера

    void PeriodicTask();

private:

    Button *btnStart = nullptr;       // Запуск развёртки
    Button *btnStop = nullptr;        // Останов развёртки

    wxTextCtrl *txtPeriodScan = nullptr;    // Период запуска развёртки в миллисекундах

    void OnEventButton(wxCommandEvent &);
};
