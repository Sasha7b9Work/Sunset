// 2023/08/09 13:34:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/TextControls.h"
#include "Settings/Settings.h"


TextCtrlNumber::TextCtrlNumber(wxWindow *parent, int id, const wxString &text_val, const wxPoint &pos, const wxSize &size, int _min, int _max) :
    wxTextCtrl(parent, id, text_val, pos, size),
    min(_min),
    max(_max)
{
    Bind(wxEVT_CHAR, &TextCtrlNumber::OnEventChar, this);
}


void TextCtrlNumber::OnEventChar(wxKeyEvent &event)
{
    int code = event.GetKeyCode();

    if (code == WXK_BACK || code == WXK_DELETE ||
        code == WXK_LEFT || code == WXK_RIGHT ||
        code == WXK_HOME || code == WXK_END ||
        code == WXK_TAB || code == WXK_RETURN)
    {
        event.Skip();
        return;
    }
    else if ((code >= '0' && code <= '9'))
    {
        wxString str = GetValue();
        str += (char)(code |= 0x30);

        long value = 0;

        str.ToLong(&value);

        if (value >= min && value <= max)
        {
            event.Skip();
            return;
        }
    }

    wxBell();
}
