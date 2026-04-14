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

    painterBMP = new PainterBMP(this, wxDefaultSize, file_jack_bmp, { 241, 241, 241 });

    if (choices)
    {
        combo = new ButtonsCombo(this, "", WIDTH_COMBO - 60, *choices, *choices, 1, "");
    }
}


PanelTests::PanelTests(wxWindow *parent) : Panel(parent, L("Тесты"))
{
    wxPanel::SetName("PanelTests");

    ThePanelTests = this;

    StaticBox *boxTest = new StaticBox(this, L("Тест"), { 100, 90 });

    {
        wxArrayString choices;
        choices.Add("IdVd");
        choices.Add("_USER");

        comboTest = new ButtonsCombo(boxTest, "", 100, choices, choices, 1, "comboTest");

        btnLoad = new wxButton(boxTest, wxID_ANY, L("Загрузить"), wxDefaultPosition, { 100, 30 });

        btnLoad->Hide();
    }

    boxTest->SetFont(StaticBox::TitleFont());

    StaticBox *boxCommutation = new StaticBox(this, L("Коммутация"),
        { 100, 50 - boxTest->GetPosition().y - boxTest->GetSize().y - SD::DSBY() });

    {
        wxArrayString choices;
        choices.Add(wxT("внутренняя"));
        choices.Add(wxT("внешняя"));

        int y = 20;

        comboCommutation = new ButtonsCombo(boxCommutation, L("Тип"), WIDTH_COMBO, choices, choices, 1, "comboCommutation");

        choices.clear();
        choices.Add(wxT("канал C"));
        choices.Add(wxT("канал B"));

        y += 40;

        jacks[Chan::_C] = new FullJack(Chan::_C, boxCommutation, "sch/jacks/jack_C.bmp", &choices);

        int dy = 35;

        y += dy;

        jacks[Chan::_B] = new FullJack(Chan::_B, boxCommutation, "sch/jacks/jack_B.bmp", &choices);

        y += dy;

        jacks[Chan::_S] = new FullJack(Chan::_S, boxCommutation, "sch/jacks/jack_S.bmp", &choices);

        y += dy;

        jacks[Chan::_E] = new FullJack(Chan::_E, boxCommutation, "sch/jacks/jack_E.bmp");

        choices.clear();
        choices.Add(wxT("C"));
        choices.Add(wxT("B"));
        choices.Add(wxT("E"));

        y = 53;
        dy = 52;

        painter = new PainterScheme(boxCommutation, { 170, 130 }, boxCommutation->GetBackgroundColour());

        {
            wxArrayString files;

            for (int i = 0; i < Category::Count; i++)
            {
                files.push_back(wxString::Format("sch/cat%d.bmp", i + 1));
            }

            wxArrayString tooltips =
            {
                L("Биполярный NPN-транзистор (трёхполюсный)"),
                L("Биполярный PNP-транзистор (трёхполюсный)"),
                L("Полевой или МОП транзистор NMOS (трёхполюсный)"),
                L("Полевой или МОП транзистор PMOS (трёхполюсный)"),
                L("Биполярный NPN-транзистор (четырёхполюсный)"),
                L("Биполярный PNP-транзистор (четырёхполюсный)"),
                L("Полевой или МОП транзистор NMOS (четырёхполюсный)"),
                L("Полевой или МОП транзистор PMOS (четырёхполюсный)"),
                L("Диод"),
                L("Тиристор"),
                L("Резистор"),
                L("Конденсатор")
            };

            comboCategory = new BmpButtonsCombo(painter, "Категория", { 55, 65 }, files, tooltips, 0, 4, "comboCategory");

            comboCategory->Show(false);
        }

        painter->Bind(wxEVT_ENTER_WINDOW, &PanelTests::OnEventMouseEnter, this);
        painter->Bind(wxEVT_LEAVE_WINDOW, &PanelTests::OnEventMouseLeave, this);
        painter->Bind(wxEVT_MOTION, &PanelTests::OnEventMouseMove, this);

        int width = 45;

        combos[Chan::_C] = new ComboJack(Chan::_C, painter, "", width, choices);

        combos[Chan::_B] = new ComboJack(Chan::_B, painter, "", width, choices);

        combos[Chan::_E] = new ComboJack(Chan::_E, painter, "", width, choices);

        combos[Chan::_S] = new ComboJack(Chan::_S, painter, "", width, choices);
    }

    boxCommutation->SetFont(StaticBox::TitleFont());

    BuildPanel();

    Bind(wxEVT_BUTTON, &PanelTests::OnEventButton, this);
    Bind(wxEVT_COMBOBOX, &PanelTests::OnEventComboBox, this);

    wxSize size{ 400, 600 };
    Panel::SetMinSize(size);
    Panel::SetSize(size);
    Panel::Layout();
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
