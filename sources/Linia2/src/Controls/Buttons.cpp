// 2023/09/02 11:37:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Buttons.h"
#include "Controls/Painter.h"
#include "Utils/GlobalFunctions.h"
#include "Controls/Bitmap.h"
#include "Utils/SystemDepend.h"
#include "Application.h"


wxDEFINE_EVENT(EVT_BUTTON_BITMAP_CHOICE, ButtonBitmapChoiceEvent);


ButtonBitmap::ButtonBitmap(wxWindow *parent, const wxPoint &pos, const wxSize &size, const wxString &file_bitmap, const wxString &name) :
    wxBitmapButton(parent, wxID_ANY, wxNullBitmap, pos, size, wxBU_EXACTFIT)
{
    wxBitmapButton::SetName(parent->GetName() + "_" + name);

    SetFileBitmap(file_bitmap);

    if (bitmap.GetBitmap().IsOk())
    {
        wxBitmapButton::SetClientSize(((size == wxDefaultSize) ? bitmap.GetBitmap().GetSize() : size) + wxSize(10, 10));
    }

    wxBitmapButton::SetBackgroundColour(GetBackgroundColour().ChangeLightness(LIGHTNESS));

    wxBitmapButton::Refresh();
    wxBitmapButton::Update();

    wxWindowBase::SetCursor(wxCursor(wxCURSOR_HAND));
}


void ButtonBitmap::SetFileBitmap(const wxString &file_bitmap)
{
    bitmap = Bitmap::Get(file_bitmap);

    if (bitmap.GetBitmap().IsOk())
    {
        wxBitmapButton::SetBitmap(bitmap.GetBitmap());
    }
}


ButtonBitmapChoice::ButtonBitmapChoice(wxWindow *parent, const wxPoint &pos, const wxSize &size, const wxArrayString &_files, const wxString &_name) :
    ButtonBitmap(parent, pos, size, _files[0], _name),
    files(_files)
{
    Bind(wxEVT_BUTTON, &ButtonBitmapChoice::OnEventButton, this);
}


void ButtonBitmapChoice::OnEventButton(wxCommandEvent &event)
{
    if (event.GetId() == GetId())
    {
        choice++;

        if ((size_t)choice >= files.GetCount()) //-V201
        {
            choice = 0;
        }

        SetCurrentValue(choice);

        GF::SendCommandEvent(this, EVT_BUTTON_BITMAP_CHOICE, choice);
    }

    event.Skip();
}


void ButtonBitmapChoice::SetCurrentValue(int value)
{
    if (value < (int)files.GetCount()) //-V202
    {
        choice = value;

        SetFileBitmap(files[(size_t)choice]); //-V201
    }
    else
    {
        LOG_ERROR("Bad index");
    }
}


int ButtonBitmapChoice::GetCurrentValue() const
{
    return choice;
}
