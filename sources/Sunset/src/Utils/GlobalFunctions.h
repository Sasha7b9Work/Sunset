// 2023/08/10 19:21:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Button.h"
#include "Controls/ButtonCombo.h"
#pragma warning(push, 0)
#pragma warning(pop)


class wxRadioButton;
class wxWindow;
class wxStaticText;
class wxCheckBox;
class wxTextCtrl;
class wxSpinCtrl;


#define FIND_CHECKBUTTON(id)  GF::FindCheckButton(this, (id))
#define FIND_ANY_BUTTON(id)   GF::FindAnyButton(this, (id))
#define FIND_CHECKBOX(id)     GF::FindCheckBox(this, (id))
#define FINC_COMBOBOX(id)     GF::FindComboBox(this, (id))
#define FINC_TEXTCTRL(id)     GF::FindTextCtrl(this, (id))
#define FIND_SPINCTRL(id)     GF::FindSpinCtrl(this, (id))


namespace GF
{
    // Возвращает каталог, где хранятся файлы *.mod и создаёт его, если таковой не существует
    wxString DirForModFiles();

    // Центрировать text относительно border
    void HorizontalAlignCenter(wxStaticText *label, wxWindow *border);

    wxStaticText *FindStaticText(wxWindow *, int id);

    wxAnyButton *FindAnyButton(wxWindow *, int id);

    wxToggleButton *FindToggleButton(wxWindow *, int id);

    wxRadioButton *FindRadioButton(const wxWindow *, int id);

    wxCheckBox *FindCheckBox(wxWindow *, int id);

    ButtonsCombo *FindComboBox(const wxWindow *, int id);

    wxTextCtrl *FindTextCtrl(wxWindow *, int id);

    wxSpinCtrl *FindSpinCtrl(wxWindow *, int id);

    void SendCommandEvent(wxWindow *, int id, int type_event, int int_value);
    void SendCommandEvent(wxWindow *object, int type_event, int int_value);

    int IntFromTextCtrl(wxWindow *, int id);

    uint64 UInt64FromTextCtrl(wxWindow *, int id);

    void IntToTextCtrl(wxWindow *, int id, int value);

    void ShowModalInfo(wxWindow *, const wxString &);

    // В эту позицию нужно переместить окно с размерами { width, height }, чтобы оно оказалось в центре экрана
    wxPoint GetCoordCenter(const wxSize &);

    // true, если запущено на плате встраиваемого ПК
    bool IsBoardPCM();

    bool ApproxEqual(double, double);

    // Получить ip-адрес устройства, на котором запущены
    wxString GetSelfIP();
}
