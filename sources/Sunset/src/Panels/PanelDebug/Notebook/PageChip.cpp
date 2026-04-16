// 2025/6/3 14:01:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageChip.h"
#include "MainWindow.h"
#pragma warning(push, 0)
#include <wx/sizer.h>
#include <wx/menu.h>
#pragma warning(pop)

PageChip::PageChip(wxNotebook *parent, const wxString &title) :
    wxPanel(parent)
{
    wxPanel::SetName(title);

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(mainSizer);

    wxPanel::SetBackgroundColour(parent->GetBackgroundColour().ChangeLightness(110));

    Bind(wxEVT_RIGHT_DOWN, &PageChip::OnRightClick, this);
}


void PageChip::AppendRegister(Register *reg)
{
    // Добавить в сайзер, а не позиционировать вручную
    GetSizer()->Add(reg, 0, wxEXPAND | wxALL, 5);

    GetSizer()->Layout();
    Layout();
}


void PageChip::Rebuild()
{
    for (uint i = 0; i < registers.size(); i++)
    {
        Register *reg = registers[i];

        int y = 0;

        if(i != 0)
        {
            Register *prev = registers[i - 1];

            y = prev->GetPosition().y + prev->GetSize().y - 1;
        }

        reg->SetPosition({ 0, y });
    }
}


void PageChip::OnRightClick(wxMouseEvent &event)
{
    wxMenu menu;

    itemReturn = menu.Append(wxID_ANY, L("Закрыть"));

    Bind(wxEVT_MENU, &PageChip::OnMenuEvent, this);

    PopupMenu(&menu);

    event.Skip();
}


void PageChip::OnMenuEvent(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == itemReturn->GetId())
    {
        TheMainWindow->SetMode(ModeMainWindow::Standard);
    }
}


void PageChip::Pack()
{
    for (auto *reg : registers)
    {
        reg->Pack();
    }
}


void PageChip::Unpack()
{
    for (auto *reg : registers)
    {
        reg->Unpack();
    }
}


void PageChip::SendAllRegisters()
{
    for (auto *reg : registers)
    {
        reg->WriteValue();
    }
}
