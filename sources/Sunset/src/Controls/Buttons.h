// 2023/09/02 11:37:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"
#include "Controls/Bitmap.h"


struct Button : public wxButton
{
    Button(wxWindow *, const wxString &, const wxSize & = wxDefaultSize, int flags = 0);
};


struct ToggleButton : public wxToggleButton
{
    ToggleButton(wxWindow *, const wxString &, const wxSize & = wxDefaultSize, int flags = 0);
};


struct ButtonBitmap : public wxBitmapButton
{
    ButtonBitmap(wxWindow *parent, const wxSize &, const wxString &file_bimap, const wxString &name);

    void SetFileBitmap(const wxString &file_bitmap);

private:

    Bitmap bitmap;
};


// На кнопке при нажатиях переключаются картинки
struct ButtonBitmapChoice : public ButtonBitmap
{
    ButtonBitmapChoice(wxWindow *parent, const wxSize &, const wxArrayString &files, const wxString &name);

    void SetCurrentValue(int);

    int GetCurrentValue() const;

private:

    wxArrayString files;

    int choice = 0;

    void OnEventButton(wxCommandEvent &);
};


// Событие от кнопки ButtonBitmapChoice
struct ButtonBitmapChoiceEvent : public wxCommandEvent
{
    ButtonBitmapChoiceEvent(wxEventType eventType, int id) : wxCommandEvent(eventType, id) { }
    ButtonBitmapChoiceEvent(const ButtonBitmapChoiceEvent &other) : wxCommandEvent(other) { }

    virtual wxEvent *Clone() const override
    {
        return new ButtonBitmapChoiceEvent(*this);
    }
};


wxDECLARE_EVENT(EVT_BUTTON_BITMAP_CHOICE, ButtonBitmapChoiceEvent);

