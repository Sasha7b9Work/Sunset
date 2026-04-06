// 2025/8/1 21:15:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Dialog.h"


// Настройки - Запуск


class WindowLaunch : public Dialog
{
public:

    static const int WIDTH = 500;
    static const int HEIGHT = 500;

    WindowLaunch();

private:

    wxRadioButton *rbOld = nullptr;
    wxRadioButton *rbNew = nullptr;

    void OnEventRadioButton(wxCommandEvent &);
};
