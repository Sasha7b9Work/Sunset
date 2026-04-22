// 2025/10/07 12:29:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "MainPanel/PanelDebug/Notebook/PageChip.h"


class PageSource50V : public PageChip
{
public:

    PageSource50V(wxNotebook *);

    static PageSource50V *self;
};
