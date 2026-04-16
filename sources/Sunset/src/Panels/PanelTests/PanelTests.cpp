// 2025/8/9 09:54:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panels.h"
#include "Panels/PanelTests/PanelTests.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "Controls/Bitmap.h"
#include "IPPP/Tests/Tests.h"
#include "Controls/StaticBox.h"
#include "Utils/Configurator.h"
#include "System/Events.h"


PanelTests *ThePanelTests = nullptr;


PanelTests::PanelTests(PanelBoard *board) : Panel(board, L("Тесты"))
{
    wxPanel::SetName("PanelTests");

    ThePanelTests = this;
}


void PanelTests::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == btnLoad->GetId())
    {
    }
}


void PanelTests::OnEventComboBox(wxCommandEvent &event)
{
    int id = event.GetId();

    if (id == comboTest->GetId())
    {
        btnLoad->Show(comboTest->GetCurrentString() == "_USER");
    }
    else if (id == comboCategory->GetId() ||
        id == comboCommutation->GetId())
    {
        BuildPanel();

        if (id == comboCategory->GetId())
        {
            {                                                                                               // Суффикс канала C
//                char suffix_c = 'a';
//
//                if (Category::IsBCE())
//                {
//                    suffix_c = 'c';
//                }
//                else if (Category::IsGDS())
//                {
//                    suffix_c = 'd';
//                }
            }

            {                                                                                               // Суффикс канала B
//                UpdateSuffixGenerator(ThePanelChannelB->comboTypeGenerator, (Category::IsGDS() || Category::Current() == Category::Thyristor) ? 'g' : 'b');
            }

            {                                                                                               // Суффикс канала S
//                UpdateSuffixGenerator(ThePanelChannelS->comboTypeGenerator, Category::IsGDS() ? 'b' : 's');
            }
        }

        Events::ChangingTheConnectionOfTheMeasuredElement();
    }
}


void PanelTests::UpdateSuffixGenerator(ButtonsCombo *combo, char suffix)
{
    const wxArrayString &old_choices = combo->GetChoices();

    wxArrayString choices;

    for (auto elem : old_choices)
    {
        if (elem.Length() == 1)
        {
            choices.push_back(elem + suffix);
        }
        else
        {
            wxString choice = elem;
            choice[1] = suffix;
            choices.push_back(choice);
        }
    }

    combo->SetChoices(choices, choices);
}


void PanelTests::BuildPanel()
{
}


void PanelTests::Pack()
{
    comboCategory->Pack();
    comboCommutation->Pack();
    for (int i = 0; i < Chan::Count; i++)
    {
    }
}


void PanelTests::Unpack()
{
    comboCategory->Unpack();
    wxYield();
    comboCommutation->Unpack();
    wxYield();
    for (int i = 0; i < Chan::Count; i++)
    {
    }
}


void PanelTests::OnEventMouseEnter(wxMouseEvent &)
{

}


void PanelTests::OnEventMouseLeave(wxMouseEvent &event)
{
    wxRect rect = comboCategory->GetRect();

    wxPoint pos = event.GetPosition();

    if (!rect.Contains(pos))
    {
        comboCategory->Hide();
    }
}


void PanelTests::OnEventMouseMove(wxMouseEvent &event)
{
    wxRect rect = comboCategory->GetRect();

    wxPoint pos = event.GetPosition();

    if (rect.Contains(pos))
    {
        comboCategory->Show();
    }
    else
    {
        comboCategory->Hide();
    }
}


void PanelTests::CallbackOnEventSize()
{

}
