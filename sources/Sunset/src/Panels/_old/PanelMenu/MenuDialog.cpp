// 2025/7/12 10:56:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/MenuDialog.h"
#include "Utils/GlobalFunctions.h"
#include "MainWindow.h"


MenuDialog::MenuDialog(const wxString &title, int width_button, const wxArrayInt &delimit,
    const wxString &btn1, void (*func1)(),
    const wxString &btn2, void (*func2)(),
    const wxString &btn3, void (*func3)(),
    const wxString &btn4, void (*func4)(),
    const wxString &btn5, void (*func5)(),
    const wxString &btn6, void (*func6)(),
    const wxString &btn7, void (*func7)(),
    const wxString &btn8, void (*func8)(),
    const wxString &btn9, void (*func9)()) :
    Dialog(title)
{
    {
        if (func1) buttons.emplace_back(ButtonStruct{ wxID_ANY, btn1, func1 });
        if (func2) buttons.emplace_back(ButtonStruct{ wxID_ANY, btn2, func2 });
        if (func3) buttons.emplace_back(ButtonStruct{ wxID_ANY, btn3, func3 });
        if (func4) buttons.emplace_back(ButtonStruct{ wxID_ANY, btn4, func4 });
        if (func5) buttons.emplace_back(ButtonStruct{ wxID_ANY, btn5, func5 });
        if (func6) buttons.emplace_back(ButtonStruct{ wxID_ANY, btn6, func6 });
        if (func7) buttons.emplace_back(ButtonStruct{ wxID_ANY, btn7, func7 });
        if (func8) buttons.emplace_back(ButtonStruct{ wxID_ANY, btn8, func8 });
        if (func9) buttons.emplace_back(ButtonStruct{ wxID_ANY, btn9, func9 });
    }

    int height_delim = 10;

    wxSize client_size{ width_button + 10, TopY() + (BUTTON_HEIGHT + 5) * (int)buttons.size() + 10 + height_delim * (int)delimit.GetCount()};

    Dialog::SetClientSize(client_size);

    int y = TopY() + 5;

    for (uint i = 0; i < buttons.size(); i++)
    {
        wxButton *btn = new wxButton(MainWidget(), wxID_ANY, buttons[i].label, { 5, y }, { width_button, BUTTON_HEIGHT });
        y += BUTTON_HEIGHT + 5;
        if (ConsistIndex(delimit, (int)i))
        {
            y += height_delim;
        }
        buttons[i].id = btn->GetId();
    } //-V773

    Bind(wxEVT_BUTTON, &MenuDialog::OnEventButton, this);
}


void MenuDialog::OnEventButton(wxCommandEvent &event)
{
    for (auto &btn : buttons)
    {
        if (btn.id == event.GetId())
        {
            btn.func();
            break;
        }
    }
}


bool MenuDialog::ConsistIndex(const wxArrayInt &array, int index)
{
    for (int elem : array)
    {
        if (elem == index)
        {
            return true;
        }
    }

    return false;
}


wxButton *MenuDialog::FindButton(const wxString &label)
{
    for (auto &btn : buttons)
    {
        wxAnyButton *button = GF::FindAnyButton(MainWidget(), btn.id);

        if (button->GetLabel() == label)
        {
            return (wxButton *)button;
        }
    }

    return nullptr;
}
