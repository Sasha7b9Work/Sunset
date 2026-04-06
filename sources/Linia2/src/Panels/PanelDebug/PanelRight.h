// 2026/01/24 21:41:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/ControlDataFPGA.h"

class PanelRight : public wxPanel
{
public:

    PanelRight(wxWindow *);

    ControlDataFPGA *data[5];           // Отображение данных, считанных c платы контроллера

    void PeriodicTask();

private:

    wxButton *btnReturn = nullptr;      // Выход в основную панель
    wxButton *btnStart = nullptr;       // Запуск развёртки
    wxButton *btnStop = nullptr;        // Останов развёртки

    wxTextCtrl *txtPeriodScan = nullptr;    // Период запуска развёртки в миллисекундах

    void OnEventButton(wxCommandEvent &);
};
