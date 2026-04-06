// 2025/7/12 10:56:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"


class MenuDialog : public Dialog
{
public:

    MenuDialog(const wxString &title, int width_button,
        const wxArrayInt &delimit,                              // Перечислены номера кнопок, после которых нужно вставлять разделитель
        const wxString &btn1, void (*func1)(),
        const wxString &btn2 = "", void (*func2)() = nullptr,
        const wxString &btn3 = "", void (*func3)() = nullptr,
        const wxString &btn4 = "", void (*func4)() = nullptr,
        const wxString &btn5 = "", void (*func5)() = nullptr,
        const wxString &btn6 = "", void (*func6)() = nullptr,
        const wxString &btn7 = "", void (*func7)() = nullptr,
        const wxString &btn8 = "", void (*func8)() = nullptr,
        const wxString &btn9 = "", void (*func9)() = nullptr
    );

protected:

    wxButton *FindButton(const wxString &label);

private:

    struct ButtonStruct
    {
        int      id;
        wxString label;
        void     (*func)();
    };

    std::vector<ButtonStruct> buttons;

    void OnEventButton(wxCommandEvent &);

    // Возвращает true, если в массиве содержится данный индекс
    bool ConsistIndex(const wxArrayInt &, int);
};
