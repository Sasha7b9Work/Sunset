// 2025/8/9 11:41:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Button.h"


// Кнопка с рисунком, по нажатию на которую открывается окно с другими выборами


class BmpButtonsCombo : public ButtonBitmap
{
public:

    BmpButtonsCombo(wxWindow *parent, const wxString &title, const wxSize &,
        const wxArrayString &files,             // Эти изображения будут на кнопках
        const wxArrayString &tooltips,
        int num_file,                           // Эта изображение будет на главной кнопке
        int buttons_in_row,                     // В каждом ряду будет расположено столько кнопок
        const wxString &name);

    void SetCurrentChoice(int);

    int GetCurrentChoice() const;

    void Pack();
    void Unpack();

private:

    int current_choice = -1;

    wxString title;
    wxArrayString files;
    wxArrayString m_tooltips;
    int buttons_in_row;

    void OnButtonClicked(wxCommandEvent &);
};
