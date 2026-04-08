// 2025/6/3 22:47:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/PagesBlocks/PageChip.h"


class PageAD5697 : public PageChip
{
public:
    PageAD5697(wxNotebook *parent);

    static PageAD5697 *self;
};
