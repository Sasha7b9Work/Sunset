// 2025/7/12 18:18:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelMenu/MenuDialog.h"


class DialogReport : public MenuDialog
{
public:

    DialogReport();

    static DialogReport *self;

private:

    static const wxString BTN_NEW;
    static const wxString BTN_SAVE;
    static const wxString BTN_DELETE;
    static const wxString BTN_EXPAND;
    static const wxString BTN_COLLAPSE;
    static const wxString BTN_CLOSE;
};
