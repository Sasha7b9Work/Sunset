// 2026/04/29 11:33:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Panel.h"


class PanelCategory : public ScrolledPanel
{
public:

    PanelCategory(wxWindow *, PanelCategory *&);

    // Обновить состояние в соответствии с библиотеками
    void UpdateState();
};
