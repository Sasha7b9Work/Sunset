// 2025/6/3 14:01:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageChip.h"
#include "MainWindow.h"


PageChip::PageChip(wxNotebook *parent, const wxString &title) :
    wxPanel(parent)
{
    wxPanel::SetName(title);

    wxPanel::SetBackgroundColour(parent->GetBackgroundColour().ChangeLightness(110));

    Bind(wxEVT_RIGHT_DOWN, &PageChip::OnRightClick, this);
}


void PageChip::AppendRegister(Register *reg)
{
    int y = 0;

    if (registers.size())
    {
        y = registers[registers.size() - 1]->GetPosition().y + registers[registers.size() - 1]->GetSize().y - 1;
    }

    reg->SetPosition({ 0, y });

    registers.push_back(reg);
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
