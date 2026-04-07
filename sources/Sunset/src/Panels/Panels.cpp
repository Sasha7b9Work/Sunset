// 2026/04/07 09:07:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panels.h"


PanelBoard::PanelBoard(wxWindow *parent) :
    wxPanel(parent)
{
    // Создаём главный вертикальный sizer
    m_mainSizer = new wxBoxSizer(wxVERTICAL);

    // === Верхняя область с кнопками ===
    wxPanel *topPanel = new wxPanel(this, wxID_ANY);
    m_topSizer = new wxBoxSizer(wxHORIZONTAL);
    topPanel->SetSizer(m_topSizer);

    // === Центральная область (динамическое содержимое) ===
    m_centerContainer = new wxPanel(this, wxID_ANY);
    m_centerSizer = new wxBoxSizer(wxVERTICAL);
    m_centerContainer->SetSizer(m_centerSizer);

    // === Нижняя область с кнопками ===
    wxPanel *bottomPanel = new wxPanel(this, wxID_ANY);
    m_bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomPanel->SetSizer(m_bottomSizer);

    // Добавляем все области в главный sizer
    // Пропорции: 0 (минимальный размер), 1 (растягивается), 0 (минимальный размер)
    m_mainSizer->Add(topPanel, 0, wxEXPAND | wxALL, 5);
    m_mainSizer->Add(m_centerContainer, 1, wxEXPAND | wxALL, 5);
    m_mainSizer->Add(bottomPanel, 0, wxEXPAND | wxALL, 5);

    SetSizer(m_mainSizer);
}


void PanelBoard::AddPanel(Panel *panel)
{
    // Отключаем родителя у панели (если был)
    if (panel->GetParent() != m_centerContainer)
    {
        panel->Reparent(m_centerContainer);
    }

    // Изначально все панели скрыты
    panel->Hide();

    // Добавляем в sizer
    m_centerSizer->Add(panel, 1, wxEXPAND);

    // Сохраняем в список
    m_contentPanels.push_back(panel);
//    m_panelNames.push_back(name);

        // ★ НОВОЕ: автоматически добавляем кнопку в верхнюю ячейку
    wxString buttonLabel = wxString::Format("Panel %d", (int)m_contentPanels.size());
    AddTopButton(buttonLabel, (wxObject *)panel); // сохраняем указатель на панель

    // Если это первая панель - показываем её
    if (m_contentPanels.size() == 1)
    {
        ShowPanel(0);
    }

    m_centerSizer->Layout();
}


void PanelBoard::ShowPanel(int index)
{
    if (index < 0 || index >= static_cast<int>(m_contentPanels.size()))
    {
        return;
    }

    ShowPanel(m_contentPanels[(size_t)index]);
}


void PanelBoard::ShowPanel(wxPanel *panel)
{
    // Скрываем текущую панель
    if (m_currentPanel)
    {
        m_currentPanel->Hide();
    }

    // Показываем новую панель
    m_currentPanel = panel;
    m_currentPanel->Show();

    // Обновляем макет
    m_centerSizer->Layout();
    m_centerContainer->Layout();
    Layout();

//    // Отправляем событие об изменении панели
//    wxCommandEvent event(wxEVT_BUTTON, GetId());
//    event.SetEventObject(this);
//    event.SetInt(GetPanelIndex(m_currentPanel));
//    GetEventHandler()->ProcessEvent(event);
}


void PanelBoard::AddTopButton(const wxString &label, wxObject *eventUserData)
{
    wxButton *btn = new wxButton(static_cast<wxPanel *>(m_topSizer->GetContainingWindow()),
        wxID_ANY, label);
    btn->SetClientData(eventUserData);
    m_topSizer->Add(btn, 0, wxRIGHT | wxTOP | wxBOTTOM, 5);
    m_topSizer->Layout();
}


Panel::Panel(wxWindow *parent) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxSUNKEN_BORDER)
{
}
