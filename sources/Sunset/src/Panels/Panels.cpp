// 2026/04/07 09:07:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panels.h"
#include "Controls/Buttons.h"


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

    // === Нижняя область с кнопками ===
    wxPanel *bottomPanel = new wxPanel(this, wxID_ANY);
    bottomPanel->SetMinSize(wxSize(-1, 30));
    bottomPanel->SetMaxSize(wxSize(-1, 30));
    bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomPanel->SetSizer(bottomSizer);

    // Добавляем все области в главный sizer
    // Пропорции: 0 (минимальный размер), 1 (растягивается), 0 (минимальный размер)
    mainSizer->Add(topPanel, 0, wxEXPAND | wxALL);
    mainSizer->Add(centerContainer, 1, wxEXPAND | wxALL);
    mainSizer->Add(bottomPanel, 0, wxEXPAND | wxALL);

    SetSizer(mainSizer);

    Bind(wxEVT_TOGGLEBUTTON, &PanelBoard::OnEventButtonToggle, this);
}


void PanelBoard::OnEventButtonToggle(wxCommandEvent &event)
{
    ToggleButton *btn = (ToggleButton *)event.GetEventObject();
    if (btn)
    {
        wxPanel *panelToShow = (wxPanel *)btn->GetClientData();
        if (panelToShow)
        {
            ShowPanel(panelToShow);
        }
    }
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
    centerSizer->Add(panel, 0, wxALIGN_CENTER);

    panels.push_back(panel);

    AddTopButton(panel->GetPanelName(), (wxObject *)panel);
    AddBottomButton(panel->GetPanelName(), (wxObject *)panel);

    // Если это первая панель - показываем её
    if (panels.size() == 1)
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
    if (index < 0 || index >= static_cast<int>(panels.size()))
    {
        return;
    }

    ShowPanel(panels[(size_t)index]);
}


void PanelBoard::ShowPanel(wxPanel *panel)
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

//    // Отправляем событие об изменении панели
//    wxCommandEvent event(wxEVT_BUTTON, GetId());
//    event.SetEventObject(this);
//    event.SetInt(GetPanelIndex(m_currentPanel));
//    GetEventHandler()->ProcessEvent(event);
}


void PanelBoard::AddTopButton(const wxString &label, wxObject *eventUserData)
{
    ToggleButton *btn = new ToggleButton(static_cast<wxPanel *>(topSizer->GetContainingWindow()),  label);
    btn->SetClientData(eventUserData);
    topSizer->Add(btn, 0, wxRIGHT | wxTOP | wxBOTTOM, 5);
    topSizer->Layout();
}

void PanelBoard::AddBottomButton(const wxString &label, wxObject *eventUserData)
{
    ToggleButton *btn = new ToggleButton(static_cast<wxPanel *>(bottomSizer->GetContainingWindow()), label);
    btn->SetClientData(eventUserData);
    bottomSizer->Add(btn, 0, wxRIGHT | wxTOP | wxBOTTOM, 5);
    bottomSizer->Layout();
}


Panel::Panel(wxWindow *parent, const wxString &_name) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxSUNKEN_BORDER),
    name(_name)
{
    Bind(wxEVT_SIZE, &Panel::OnEventSize, this);
}


const wxString &Panel::GetPanelName() const
{
    return name;
}


void Panel::OnEventSize(wxSizeEvent &event)
{
    CallbackOnEventSize();

    Refresh();

    event.Skip();
}
