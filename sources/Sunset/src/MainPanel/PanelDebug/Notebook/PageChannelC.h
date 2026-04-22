// 2025/10/07 11:52:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "MainPanel/PanelDebug/Notebook/PageChip.h"


class PageChannelC : public PageChip
{
public:

    PageChannelC(wxNotebook *);

    static PageChannelC *self;
};
