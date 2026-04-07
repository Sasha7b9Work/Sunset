// 2025/10/07 11:39:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/Notebook/PageChip.h"


class PageSource3kV : public PageChip
{
public:

    PageSource3kV(wxNotebook *);

    static PageSource3kV *self;
};
