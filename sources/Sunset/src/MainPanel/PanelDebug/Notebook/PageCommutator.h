// 2025/10/07 11:47:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "MainPanel/PanelDebug/Notebook/PageChip.h"


class PageCommutator : public PageChip
{
public:

    PageCommutator(wxNotebook *);

    static PageCommutator *self;
};
