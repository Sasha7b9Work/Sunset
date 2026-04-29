// 2026/04/08 15:31:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook.h"
#include "Controls/Buttons.h"
#pragma warning(push, 0)
#include <wx/sizer.h>
#pragma warning(pop)


PageNotebook::PageNotebook(Notebook *board, const wxString &_name) :
    Panel(board->GetCenterContainer()),
    name(_name)
{
    Panel::Bind(wxEVT_SIZE, &PageNotebook::OnEventSize, this);
}


const wxString &PageNotebook::GetPanelName() const
{
    return name;
}


void PageNotebook::OnEventSize(wxSizeEvent &event)
{
    const wxSize size = GetParent()->GetSize();

    if (size.x && size.y)
    {
        if (size != prev_size)
        {
            prev_size = size;

            SetMinSize(size);
            SetSize(size);

            Refresh();

            CallbackOnEventSize();
            Refresh();
        }
    }

    event.Skip();
}


Notebook::Notebook(wxWindow *parent) :
    Panel(parent)
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

    // Обновляем макет
    centerSizer->Layout();
    centerContainer->Layout();
    Layout();

    for (auto btn : buttons)
    {
        btn->SetValue(btn->GetClientData() == panel);
    }

//    // Принудительная перерисовка
//    currentPanel->Refresh();
//    currentPanel->Update();
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


void Notebook::SetCurrentPanelIndex(int index)
{
    if (index < 0 || index >= static_cast<int>(buttons.size()))
    {
        return;
    }

    PageNotebook *panel = (PageNotebook *)buttons[(size_t)index]->GetClientData();

    SetCurrentPanel(panel);

    wxCommandEvent event;
    event.SetEventObject(buttons[(uint64)index]);
    event.SetInt(1);
    OnEventButtonToggle(event);
}


void Notebook::AddTopButton(PageNotebook *panel)
{
    ToggleButton *btn = new ToggleButton((wxPanel *)topSizer->GetContainingWindow(), panel->GetPanelName());
    btn->SetClientData((wxObject *)panel);
    btn->Bind(wxEVT_TOGGLEBUTTON, &Notebook::OnEventButtonToggle, this);
    topSizer->Add(btn, 0, wxRIGHT | wxTOP | wxBOTTOM, 5);
    topSizer->Layout();
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
