// 2026/03/16 11:53:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/WindowCalculation/WindowCalculation.h"
#include "Settings/Settings.h"


WindowCalculation::WindowCalculation(const wxString &_title, const wxSize &_size) : DraggedDialog(_title, _size)
{
}



void WindowCalculation::CreateMainPanel2()
{
    // Создаем панель для всего содержимого
    wxPanel *mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxNO_BORDER | wxEXPAND);

    // Вертикальный сазер для главной панели
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Создаем полосу заголовка (добавляем сразу в mainSizer)
    CreateTitleBar(mainPanel, mainSizer);

    // Создаем панель с содержимым
    wxPanel *contentPanel = new wxPanel(mainPanel, wxID_ANY);
    contentPanel->SetBackgroundColour(*wxWHITE);
    mainSizer->Add(contentPanel, 1, wxEXPAND | wxALL, 5);

    // Добавляем содержимое в contentPanel
    wxBoxSizer *contentSizer = new wxBoxSizer(wxVERTICAL);
    contentPanel->SetSizer(contentSizer);

    // Заголовок секции
    wxStaticText *titleLabel = new wxStaticText(contentPanel, wxID_ANY,
        "Параметры расчета:", wxDefaultPosition, wxDefaultSize,
        wxALIGN_LEFT);
    wxFont titleFont = titleLabel->GetFont();
    titleFont.MakeBold();
    titleLabel->SetFont(titleFont);
    contentSizer->Add(titleLabel, 0, wxALL, 10);

    // Сетка для полей ввода
    wxFlexGridSizer *gridSizer = new wxFlexGridSizer(2, 5, 10);
    gridSizer->AddGrowableCol(1, 1);

    // Поле 1
    gridSizer->Add(new wxStaticText(contentPanel, wxID_ANY, "Значение 1:"),
        0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    wxTextCtrl *value1 = new wxTextCtrl(contentPanel, wxID_ANY, "100");
    gridSizer->Add(value1, 1, wxEXPAND | wxRIGHT, 10);

    // Поле 2
    gridSizer->Add(new wxStaticText(contentPanel, wxID_ANY, "Значение 2:"),
        0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    wxTextCtrl *value2 = new wxTextCtrl(contentPanel, wxID_ANY, "200");
    gridSizer->Add(value2, 1, wxEXPAND | wxRIGHT, 10);

    // Поле 3
    gridSizer->Add(new wxStaticText(contentPanel, wxID_ANY, "Значение 3:"),
        0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    wxTextCtrl *value3 = new wxTextCtrl(contentPanel, wxID_ANY, "300");
    gridSizer->Add(value3, 1, wxEXPAND | wxRIGHT, 10);

    contentSizer->Add(gridSizer, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);

    // Разделительная линия
    wxStaticLine *line = new wxStaticLine(contentPanel);
    contentSizer->Add(line, 0, wxEXPAND | wxALL, 10);

    // Результат
    wxBoxSizer *resultSizer = new wxBoxSizer(wxHORIZONTAL);
    resultSizer->Add(new wxStaticText(contentPanel, wxID_ANY, "Результат:"),
        0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    wxStaticText *result = new wxStaticText(contentPanel, wxID_ANY, "600");
    wxFont resultFont = result->GetFont();
    resultFont.MakeBold();
    resultFont.SetPointSize(resultFont.GetPointSize() + 2);
    result->SetFont(resultFont);
    result->SetForegroundColour(wxColour(0, 100, 0));
    resultSizer->Add(result, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    contentSizer->Add(resultSizer, 0, wxEXPAND | wxBOTTOM, 10);

    // Кнопки
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton *calcBtn = new wxButton(contentPanel, wxID_ANY, "Рассчитать");
    calcBtn->SetBackgroundColour(wxColour(220, 240, 220));
    buttonSizer->Add(calcBtn, 0, wxALL, 5);

    wxButton *closeBtn = new wxButton(contentPanel, wxID_ANY, "Закрыть");
    closeBtn->SetBackgroundColour(wxColour(240, 220, 220));
    buttonSizer->Add(closeBtn, 0, wxALL, 5);

    contentSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    // Обработчики кнопок
    calcBtn->Bind(wxEVT_BUTTON, [value1, value2, value3, result](wxCommandEvent &)
        {
            double v1 = wxAtof(value1->GetValue());
            double v2 = wxAtof(value2->GetValue());
            double v3 = wxAtof(value3->GetValue());
            double sum = v1 + v2 + v3;
            result->SetLabel(wxString::Format("%.2f", sum));
        });

    closeBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent &)
        {
            Close();
        });

    // Устанавливаем сазер для главной панели
    mainPanel->SetSizer(mainSizer);

    // Создаем сазер для фрейма
    wxBoxSizer *frameSizer = new wxBoxSizer(wxHORIZONTAL);
    frameSizer->Add(mainPanel, 1, wxEXPAND);
    SetSizer(frameSizer);
    Layout();
    Fit();

    // Устанавливаем минимальный размер
    SetMinSize(wxSize(350, 300));

    // Центрируем относительно родителя
    if (GetParent())
    {
        wxRect parentRect = GetParent()->GetScreenRect();
        wxFrame::SetPosition(wxPoint(parentRect.x + 50, parentRect.y + 50));
    }
}


void WindowCalculation::CreateTitleBar(wxWindow *_parent, wxBoxSizer *mainSizer)
{
    // Панель заголовка
    titleBar = new wxPanel(_parent, wxID_ANY,
        wxDefaultPosition, wxSize(-1, 25));
    titleBar->SetBackgroundColour(wxColour(60, 80, 120)); // Темно-синий

    // Горизонтальный сазер для заголовка
    wxBoxSizer *titleSizer = new wxBoxSizer(wxHORIZONTAL);

    // Иконка - используем существующий ART-идентификатор
    wxBitmap iconBitmap;

    // Пробуем разные варианты иконок
    iconBitmap = wxArtProvider::GetBitmap(wxART_INFORMATION, wxART_OTHER, wxSize(16, 16));
    if (!iconBitmap.IsOk())
    {
        // Если не получилось, создаем простую иконку
        iconBitmap = wxBitmap(16, 16);
        wxMemoryDC dc;
        dc.SelectObject(iconBitmap);
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(*wxWHITE_BRUSH);
        dc.DrawRectangle(0, 0, 16, 16);
        dc.DrawText("i", 4, 2);
        dc.SelectObject(wxNullBitmap);
    }

    wxStaticBitmap *icon = new wxStaticBitmap(titleBar, wxID_ANY, iconBitmap);
    titleSizer->Add(icon, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 15);

    {
        // Кнопка закрытия

        wxButton *closeBtn = new wxButton(titleBar, wxID_ANY, "x", wxDefaultPosition, wxSize(20, 20), wxBORDER_NONE);
        closeBtn->SetBackgroundColour(wxColour(180, 60, 60));
        closeBtn->SetForegroundColour(*wxWHITE);
        closeBtn->SetFont(closeBtn->GetFont().Scale(1.3f));

        closeBtn->Bind(wxEVT_ENTER_WINDOW, [closeBtn](wxMouseEvent &)
            {
                closeBtn->SetBackgroundColour(wxColour(200, 80, 80));
                closeBtn->Refresh();
            });

        closeBtn->Bind(wxEVT_LEAVE_WINDOW, [closeBtn](wxMouseEvent &)
            {
                closeBtn->SetBackgroundColour(wxColour(180, 60, 60));
                closeBtn->Refresh();
            });

        closeBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent &)
            {
                Close();
            });

        titleSizer->Add(closeBtn, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    }

    titleBar->SetSizer(titleSizer);
    mainSizer->Add(titleBar, 0, wxEXPAND);
}
