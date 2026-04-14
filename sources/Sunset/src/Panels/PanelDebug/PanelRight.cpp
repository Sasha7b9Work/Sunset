// 2026/01/24 21:42:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/PanelRight.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "IPPP/IDevice.h"
#include "Panels/PanelDebug/Notebook/PageFPGA.h"
#include "Panels/PanelDebug/Notebook/PageChannelC.h"
#include "Utils/Math.h"
#include "Utils/Timer.h"


PanelRight *ThePanelRight = nullptr;


PanelRight::PanelRight(wxWindow *parent) : wxPanel(parent)
{
    ThePanelRight = this;

    SetWindowStyle(wxBORDER_SIMPLE);

    wxSize size_button{ 75, BUTTON_HEIGHT };

    // Создание элементов управления
    btnStart = new Button{ this, L("Старт"), size_button };
    btnStart->Bind(wxEVT_BUTTON, &PanelRight::OnEventButton, this);
    btnStart->SetToolTip(L("Запуск развёртки"));

    txtPeriodScan = new wxTextCtrl{ this, wxID_ANY, "1000", wxDefaultPosition, size_button };
    txtPeriodScan->SetToolTip(L("Период запуска развёртки в миллисекундах"));

    btnStop = new Button{ this, L("Стоп"), size_button };
    btnStop->Bind(wxEVT_BUTTON, &PanelRight::OnEventButton, this);
    btnStop->SetToolTip(L("Останов развёртки"));
    btnStop->Enable(false);

    // Создание пяти ControlDataFPGA
    for (int i = 0; i < 5; i++)
    {
        data[i] = new ControlDataFPGA(this);
    }
    data[4]->SetMax((1 << 8) - 1);

    // Основной вертикальный сайзер
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Горизонтальный сайзер для верхней строки (btnStart и txtPeriodScan)
    wxBoxSizer *topLineSizer = new wxBoxSizer(wxHORIZONTAL);
    topLineSizer->Add(btnStart, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    topLineSizer->Add(txtPeriodScan, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    mainSizer->Add(topLineSizer, 0, wxALIGN_LEFT | wxTOP | wxLEFT, 10);

    // Кнопка btnStop — под btnStart (с небольшим отступом слева, чтобы визуально выровнять с btnStart)
    mainSizer->Add(btnStop, 0, wxALIGN_LEFT | wxLEFT, 15);

    // Горизонтальный сайзер для пяти ControlDataFPGA (можно разместить в несколько строк, если нужно)
    // Здесь они идут в одной горизонтальной линии (если не помещаются, можно обернуть в wxWrapSizer)
    wxBoxSizer *dataSizer = new wxBoxSizer(wxHORIZONTAL);
    for (int i = 0; i < 5; i++)
    {
        dataSizer->Add(data[i], 1, wxALL | wxEXPAND, 5);
    }
    mainSizer->Add(dataSizer, 1, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);
    Fit();
    Layout();
}


void PanelRight::PeriodicTask()
{
    static TimeMeterMS timer;

    if (timer.ElapsedMS() < 1000)
    {
        return;
    }

    timer.Reset();

    static int MAX = ((1 << 18) - 1);

    int points[MAX_NUMBER_POINTS];

    for (int d = 0; d < 4; d++)
    {
        points[0] = Math::Rand(0, MAX);

        for (int i = 1; i < MAX_NUMBER_POINTS; i++)
        {
            int step = Math::Rand(-10000, 10000);

            int new_value = points[i - 1] + step;

            if (new_value < 0 || new_value > MAX)
            {
                new_value -= 2 * step;
            }

            points[i] = new_value;
        }

        data[d]->SetData(points);
    }
}


void PanelRight::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnStart->GetId())
    {
        wxString str_value = txtPeriodScan->GetValue();
        int int_value = 0;
        str_value.ToInt(&int_value);

        PageFPGA::self->SendAllRegisters();
        PageChannelC::self->SendAllRegisters();

        IDevice::impl->SendCommand(wxString::Format(":SCAN:START %d", int_value));

        btnStart->Enable(false);
        btnStop->Enable(true);
        txtPeriodScan->Enable(false);
    }
    else if (id == btnStop->GetId())
    {
        IDevice::impl->SendCommand(":SCAN:STOP");

        btnStart->Enable(true);
        btnStop->Enable(false);
        txtPeriodScan->Enable(true);
    }
}
