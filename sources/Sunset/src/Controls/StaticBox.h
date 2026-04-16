// 2025/8/20 19:59:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/statbox.h>
#include <wx/sizer.h>
#pragma warning(pop)


class StaticBox : public wxStaticBox
{
public:
    StaticBox(wxWindow *parent, const wxString &title, const wxSize &size = wxDefaultSize) :
        wxStaticBox(parent, wxID_ANY, title, wxDefaultPosition, size, wxBORDER_NONE)
    {
        wxStaticBox::SetName(parent->GetName());

        if (font == wxNullFont)
        {
            font = GetFont();
            font.SetWeight(wxFONTWEIGHT_BOLD);
//            font.SetPointSize(font.GetPointSize() + 1);
        }
    }

    static wxFont &TitleFont()
    {
        return font;
    }

private:

    static wxFont font;
};


class StaticBoxSizer : public wxStaticBoxSizer
{
public:
    StaticBoxSizer(int type, wxWindow *parent, const wxString &title) :
        wxStaticBoxSizer(type, parent, title)
    {
#ifdef WIN32
#else
        m_staticBox->SetWindowStyle(wxBORDER_RAISED);
#endif
    }
};
