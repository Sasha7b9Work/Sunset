// 2026/04/08 15:31:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelBoard.h"


PanelBoard::PanelBoard(wxWindow *parent) :
    wxPanel(parent)
{
    // Создаём главный вертикальный sizer
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // === Верхняя область с кнопками ===
    wxPanel *topPanel = new wxPanel(this, wxID_ANY);
    topPanel->SetMinSize(wxSize(-1, 30));
    topPanel->SetMaxSize(wxSize(-1, 30));
    topSizer = new wxBoxSizer(wxHORIZONTAL);
    topPanel->SetSizer(topSizer);

    // === Центральная область (динамическое содержимое) ===
    centerContainer = new wxPanel(this, wxID_ANY);
    centerSizer = new wxBoxSizer(wxVERTICAL);
    centerContainer->SetSizer(centerSizer);

    // Добавляем все области в главный sizer
    // Пропорции: 0 (минимальный размер), 1 (растягивается), 0 (минимальный размер)
    mainSizer->Add(topPanel, 0, wxEXPAND | wxALL);
    mainSizer->Add(centerContainer, 1, wxEXPAND | wxALL);

    SetSizer(mainSizer);
}


void PanelBoard::OnEventButtonToggle(wxCommandEvent &event)
{
    ToggleButton *btn = (ToggleButton *)event.GetEventObject();

    if (btn)
    {
        if (event.GetInt() == 0)
        {
            btn->SetValue(true);
        }
        else
        {
            Panel *panel = (Panel *)btn->GetClientData();

            if (panel)
            {
                ShowPanel(panel);
            }
        }
    }

    event.Skip();
}


void PanelBoard::AddPanel(Panel *panel)
{
    // Отключаем родителя у панели (если был)
    if (panel->GetParent() != centerContainer)
    {
        panel->Reparent(centerContainer);
    }

    // Изначально все панели скрыты
    panel->Hide();

    // Добавляем в sizer
    centerSizer->Add(panel, 0, wxALIGN_TOP | wxALIGN_LEFT);

    AddTopButton(panel);

    // Если это первая панель - показываем её
    if (buttons.size() == 1)
    {
        ShowPanel(0);
    }

    centerSizer->Layout();

    wxSize newSize = centerContainer->GetSize();
    panel->SetSize(newSize);

    // Отправляем событие изменения размера панели
    wxSizeEvent sizeEvent(newSize, panel->GetId());
    sizeEvent.SetEventObject(panel);
    panel->GetEventHandler()->ProcessEvent(sizeEvent);
}


void PanelBoard::ShowPanel(int index)
{
    if (index < 0 || index >= static_cast<int>(buttons.size()))
    {
        return;
    }

    Panel *panel = (Panel *)buttons[(size_t)index]->GetClientData();

    ShowPanel(panel);
}


void PanelBoard::ShowPanel(Panel *panel)
{
    // Скрываем текущую панель
    if (currentPanel)
    {
        currentPanel->Hide();
    }

    // Показываем новую панель
    currentPanel = panel;
    currentPanel->Show();

    // Обновляем макет
    centerSizer->Layout();
    centerContainer->Layout();
    Layout();

    for (auto btn : buttons)
    {
        btn->SetValue(btn->GetClientData() == panel);
    }

    //    // Отправляем событие об изменении панели
    //    wxCommandEvent event(wxEVT_BUTTON, GetId());
    //    event.SetEventObject(this);
    //    event.SetInt(GetPanelIndex(m_currentPanel));
    //    GetEventHandler()->ProcessEvent(event);
}


void PanelBoard::AddTopButton(Panel *panel)
{
    ToggleButton *btn = new ToggleButton((wxPanel *)topSizer->GetContainingWindow(), panel->GetPanelName());
    btn->SetClientData((wxObject *)panel);
    btn->Bind(wxEVT_TOGGLEBUTTON, &PanelBoard::OnEventButtonToggle, this);
    topSizer->Add(btn, 0, wxRIGHT | wxTOP | wxBOTTOM, 5);
    topSizer->Layout();
    buttons.push_back(btn);
}
