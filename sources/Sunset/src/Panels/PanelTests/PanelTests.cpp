// 2025/8/9 09:54:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panels.h"
#include "Panels/PanelTests/PanelTests.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "Panels/PanelTests/PainterScheme.h"
#include "Controls/Bitmap.h"
#include "IPPP/Tests/Tests.h"
#include "Controls/StaticBox.h"
#include "Utils/Configurator.h"
#include "System/Events.h"
#include "Panels/PanelTests/WindowLibraryTests.h"


PanelTests *ThePanelTests = nullptr;


ComboJack::ComboJack(Chan::E ch, wxWindow *parent, const wxString &title, int width, const wxArrayString &labels) :
    ButtonsCombo(parent, title, width, labels, labels, 1, parent->GetName() + wxString::Format("_comboJack%d", (int)ch)),
    channel(ch)
{

}


FullJack::FullJack(Chan::E ch, wxWindow *parent, pchar file_jack_bmp, const wxArrayString *choices) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, { 180, 30 }),
    channel(ch)
{
    wxPanel::SetName(parent->GetName() + wxString::Format("_fullJack%d", (int)ch));

    painterBMP = new PainterBMP(this, wxDefaultPosition, wxDefaultSize, file_jack_bmp, { 241, 241, 241 });

    if (choices)
    {
        combo = new ButtonsCombo(this, "", WIDTH_COMBO - 60, *choices, *choices, 1, "");
    }
}


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
        WindowLibraryTests().ShowOnWindow(btnLoad);
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
//    ThePanelConfig->btnChannelB->Enable(ThePanelChannelB->IsEnabled());
//    ThePanelConfig->btnChannelS->Enable(ThePanelChannelS->IsEnabled());

    for (auto _jack : jacks)
    {
        _jack->TuneState();
    }

    for (auto _combo : combos)
    {
        _combo->TuneState();
    }

    painter->Build();
}


void ComboJack::TuneState()
{
    SetChoices();

    SetVisibility();

    Enable(!TypeCommutation::IsInternal());
}


void FullJack::TuneState()
{
    SetChoices();

    SetVisibility();

    if (combo)
    {
        combo->Enable(TypeCommutation::IsInternal());
    }
}


void ComboJack::SetChoices()
{
    Category::E cat = Category::Current();

    wxArrayString choices;

    if (cat == Category::Diod)
    {
        StateJack::PrepareArray(choices, StateJack::_C, StateJack::_E);
        ButtonsCombo::SetChoices(choices, choices);
        SetChoice(StateJack::_C);
    }
    else
    {
        StateJack::PrepareArray(choices, StateJack::_C, StateJack::_B, StateJack::_E, StateJack::Break);
        ButtonsCombo::SetChoices(choices, choices);
        SetChoice(StateJack::_C);
    }
}


void FullJack::SetChoices()
{
    Category::E cat = Category::Current();

    if (channel == Chan::_C)
    {
        if (cat == Category::Diod)
        {
            wxArrayString choices;
            StateJack::PrepareArray(choices, StateJack::Break, StateJack::_C);
            combo->SetChoices(choices, choices);
            SetChoice(StateJack::_C);
        }
    }
}


void FullJack::Pack()
{
    if (combo)
    {
        Config::Write(GetName(), combo->GetCurrentString());
    }
}


void FullJack::Unpack()
{
    if (combo)
    {
        combo->SetCurrentString(Config::ReadString(GetName()));
    }
}


void FullJack::SetChoice(StateJack::E state)
{
    combo->SetCurrentString(StateJack::NameHardware(state));
}


void ComboJack::SetChoice(StateJack::E state)
{
    ButtonsCombo::SetCurrentString(StateJack::NameHardware(state));
}


void FullJack::SetVisibility()
{
    Show(Chan(channel).IsVisible());
}


void ComboJack::SetVisibility()
{
    Show(Chan(channel).IsVisible());
}


void PanelTests::Pack()
{
    comboCategory->Pack();
    comboCommutation->Pack();
    for (int i = 0; i < Chan::Count; i++)
    {
        combos[i]->Pack();
        jacks[i]->Pack();
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
        combos[i]->Unpack();
        jacks[i]->Unpack();
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
