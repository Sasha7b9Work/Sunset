// 2025/10/07 12:21:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/Notebook/PageChip.h"


class PageChannelS : public PageChip
{
public:

    PageChannelS(wxNotebook *);

    static PageChannelS *self;
};
