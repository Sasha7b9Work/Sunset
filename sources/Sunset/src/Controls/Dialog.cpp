// 2025/7/25 12:23:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Dialog.h"
#include "Utils/SystemDepend.h"

Dialog::Dialog(const wxString &_title, const wxPoint &_pos, const wxSize &_size) :
    DraggedDialog(_title, _size)
{
    wxFrame::SetPosition(_pos);
    Bind(wxEVT_CHAR_HOOK, &Dialog::OnKeyDown, this);
}


int Dialog::CreateLabelGroup(wxWindow *parent, int y, const wxString &label)
{
    y += 15;

    new wxStaticLine(parent, wxID_ANY, wxDefaultPosition, { 100, -1 }, wxLI_HORIZONTAL);

    y += 7;

    new wxStaticText(parent, wxID_ANY, label);

    return y + 25;
}


int Dialog::CreateRadioButton(wxWindow *parent, int y, wxRadioButton **rb, const wxString &label)
{
    *rb = new wxRadioButton(parent, wxID_ANY, label);

    return y + 20;
}


void Dialog::ShowOnWindow(wxWindow *window)
{
    wxSize size = window->GetSize();

    wxPoint pos = window->ClientToScreen(wxPoint(size.x / 2, size.y / 2));

    pos -= GetSize() / 2;

    if (GF::IsBoardPCM())
    {

    }
    else
    {
        Fit();
        Layout();

        int win_bottom = TheMainWindow->GetRect().GetBottom();
        int win_left = TheMainWindow->GetRect().GetLeft();

        int bottom = pos.y + GetSize().y;
        int left = pos.x;

        if (bottom >= win_bottom)
        {
            pos.y = win_bottom - GetSize().y;
        }
        if (left <= win_left)
        {
            pos.x = win_left;
        }
    }

    wxFrame::SetPosition(pos);

    DraggedDialog::ShowModal();
}


void Dialog::OnKeyDown(wxKeyEvent &event)
{
    if (event.GetKeyCode() == WXK_ESCAPE)
    {
        DraggedDialog::CloseModal();
    }
    else
    {
        event.Skip();
    }
}
