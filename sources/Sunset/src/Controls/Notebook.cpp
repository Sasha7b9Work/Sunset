// 2026/04/08 15:31:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook.h"
#include "Controls/Button.h"
#pragma warning(push, 0)
#include <wx/sizer.h>
#pragma warning(pop)


PageNotebook::PageNotebook(Notebook *board, const wxString &_name) :
    Panel(board->GetCenterContainer()),
    name(_name)
{
}


const wxString &PageNotebook::GetPanelName() const
{
    return name;
}


Notebook::Notebook(wxWindow *parent) :
    Panel(parent)
{
    // Создаём главный вертикальный sizer
    mainSizer = new wxBoxSizer(wxVERTICAL);

    // === Верхняя область с кнопками ===
    wxPanel *panel_buttons = new wxPanel(this, wxID_ANY);
    panel_buttons->SetMinSize(wxSize(-1, 30));
    panel_buttons->SetMaxSize(wxSize(-1, 30));
    sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
    panel_buttons->SetSizer(sizer_buttons);

    // === Центральная область (динамическое содержимое) ===
    centerContainer = new wxPanel(this, wxID_ANY);
    centerSizer = new wxBoxSizer(wxVERTICAL);
    centerContainer->SetSizer(centerSizer);

    // Добавляем все области в главный sizer
    // Пропорции: 0 (минимальный размер), 1 (растягивается), 0 (минимальный размер)
    mainSizer->Add(panel_buttons, 0, wxEXPAND | wxALL);
    mainSizer->Add(centerContainer, 1, wxEXPAND | wxALL);

    SetSizer(mainSizer);
}


void Notebook::OnEventButtonToggle(wxCommandEvent &event)
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
            PageNotebook *panel = (PageNotebook *)btn->GetClientData();

            if (panel)
            {
                SetCurrentPanel(panel);
            }
        }
    }

    event.Skip();
}


void Notebook::SetCurrentPanel(PageNotebook *panel)
{
    // Скрываем текущую панель
    if (currentPanel)
    {
        currentPanel->Hide();
    }

    // Показываем новую панель
    currentPanel = panel;
    currentPanel->Show();

    panel->Layout();
    if (panel->GetSizer())
    {
        panel->GetSizer()->Layout();
    }
    // Обновляем макет
    centerSizer->Layout();
    centerContainer->Layout();
    Layout();

    for (auto btn : buttons)
    {
        btn->SetValue(btn->GetClientData() == panel);
    }

    currentPanel->Refresh();
    currentPanel->Update();
}



void Notebook::AddPanel(PageNotebook *panel)
{
    // Изначально все панели скрыты
    panel->Hide();

    // Добавляем в sizer
    centerSizer->Add(panel, 1, wxEXPAND | wxALL, 0);

    AddTopButton(panel);

    centerSizer->Layout();

    wxSize newSize = centerContainer->GetSize();
    panel->SetSize(newSize);
}


void Notebook::AddTopButton(PageNotebook *panel)
{
    ToggleButton *btn = new ToggleButton((wxPanel *)sizer_buttons->GetContainingWindow(), panel->GetPanelName());
    btn->SetClientData((wxObject *)panel);
    btn->Bind(wxEVT_TOGGLEBUTTON, &Notebook::OnEventButtonToggle, this);
    sizer_buttons->Add(btn, 0, wxRIGHT | wxTOP | wxBOTTOM, 5);
    sizer_buttons->Layout();
    buttons.push_back(btn);
}


int Notebook::GetCurrentPanelIndex() const
{
    return GetPanelIndex(currentPanel);
}


int Notebook::GetPanelIndex(PageNotebook *panel) const
{
    for (uint i = 0; i < buttons.size(); i++)
    {
        if (buttons[i]->GetClientData() == panel)
        {
            return (int)i;
        }
    }

    return 0;
}
