// 2025/7/23 14:20:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/SpinBox.h"
#include "Controls/Button.h"
#pragma warning(push, 0)
#include <wx/button.h>
#include <wx/textctrl.h>
#pragma warning(pop)


SpinBox::SpinBox(wxWindow *parent, const wxSize &size, int _min, int _max) :
    Panel(parent),
    m_min(_min),
    m_max(_max)
{
    Panel::SetSize(size);

    const int width_btn = 15;

    wxSize size_text = size;
    size_text.x -= width_btn;

    text = new wxTextCtrl(this, wxID_ANY, "", { 0, 0 }, size_text);

    text->SetEditable(false);

    SetValue(m_min);

    wxSize size_button{ width_btn, 11 };

    btnMore = new Button(this, ".", size_button);
    btnMore->Bind(wxEVT_BUTTON, &SpinBox::OnEventButton, this);

    btnLess = new Button(this, ".", size_button);

    btnLess->Bind(wxEVT_BUTTON, &SpinBox::OnEventButton, this);
}


void SpinBox::OnEventButton(wxCommandEvent &event)
{
    int id = event.GetId();

    int value = GetValue();

    if (id == btnMore->GetId())
    {
        if (value < m_max)
        {
            SetValue(value + 1);
        }
    }
    else if (id == btnLess->GetId())
    {
        if (value > m_min)
        {
            SetValue(value - 1);
        }
    }

    event.Skip();
}


int SpinBox::GetValue() const
{
    int result = 0;

    text->GetValue().ToInt(&result);

    return result;
}


void SpinBox::SetValue(int value)
{
    text->SetValue(wxString::Format("%d", value));
}
