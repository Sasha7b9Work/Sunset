// 2025/7/12 17:36:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelMenu/MenuDialog.h"


class DialogTable : public MenuDialog
{
public:

    DialogTable();

    static DialogTable *self;

private:

    static const wxString BTN_SAVE;
    static const wxString BTN_DELETE;
    static const wxString BTN_EXPAND;
    static const wxString BTN_COLLAPSE;
};
