// 2025/7/12 11:08:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelMenu/MenuDialog.h"


class DialogModels : public MenuDialog
{
public:
 
    DialogModels();

    void UpdateStateControls();

private:

    static const wxString BTN_NEW;
    static const wxString BTN_OPEN;
    static const wxString BTN_CLOSE;
    static const wxString BTN_SAVE;
    static const wxString BTN_SAVE_AS;
    static const wxString BTN_DELETE;
};
