// 2025/8/9 11:41:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Buttons.h"


// Кнопка с рисунком на ней
class DrawingButton : public Button
{
public:
    DrawingButton(wxWindow *, const wxString &, const wxSize &, const wxString &name_file = "");

private:

    wxString file_name;

    void OnPaint(wxPaintEvent &);
};


// Кнопка с рисунком, по нажатию на которую открывается окно с другими выборами
class ButtonsCombo : public DrawingButton
{
    friend class ButtonPopup;

public:

    struct Type
    {
        enum E
        {
            Text,
            Bitmap
        };
    };

    explicit ButtonsCombo(wxWindow *parent, const wxString &title, int width,
        const wxArrayString &labels,
        const wxArrayString &tooltips,
        int buttons_in_row,                     // В каждом ряду будет расположено столько кнопок
        const wxString &name,
        Type::E type = Type::Text);

    void SetCurrentSelection(int);
    int GetCurrentSelection() const;

    void SetCurrentString(const wxString &);
    wxString GetCurrentString() const;

    void SetLastSelection();

    void SetChoices(const wxArrayString &labels, const wxArrayString &tooltips);
    const wxArrayString &GetChoices() const;

    void SetTitle(const wxString &);

    void Pack();
    void Unpack();

    int NumChoices() const;

    virtual void SetMyToolTip(const wxString &);
    wxString GetMyToolTip() const;

    bool left_align = false;            // Если true - надписи на выборах будут выравниваться по левому краю

protected:

    wxString myToolTip;
    bool insert_empty = false;          // true, если перед первым элементом нужно вставлять пустые элементы (для диапазонов, чтобы они согласованно располагались по столбцам)
    wxString title;

    // Между строками будут пробелы таким образом, что строки будут по краям кнопки
    virtual void SetExtendedLabel(const wxString &, const wxString &);
    void SetExtendedLabel(const wxString &, int num_spaces, const wxString &);

private:
    wxColour colorBackground;
    wxColour colorForeground;
    int current_choice = -1;            // Номер реального выбора
    wxArrayString labels;
    wxArrayString tooltips;
    int buttons_in_row = 0;
    bool left_part_clicked = true;      // Если true, то кликнули мышкой в левой части кнопки
    int num_empties = 0;                // Столько добавлено пустых полей перед первым вариантом, чтобы выровнять столбцы по значениям

    void OnButtonClicked(wxCommandEvent &);
    void OnMouseDown(wxMouseEvent &);
};


// Кнопка выбора диапазона
class ButtonsComboRange : public ButtonsCombo
{
public:

    ButtonsComboRange(wxWindow *parent, const wxString &title, int width,
        const wxArrayString &labels,
        const wxArrayString &tooltips,
        const wxString &name);
};
