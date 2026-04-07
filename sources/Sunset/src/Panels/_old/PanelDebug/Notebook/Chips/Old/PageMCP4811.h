// 2025/6/3 22:33:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/PagesBlocks/PageChip.h"


class PageMCP4811 : public PageChip
{
public:
    PageMCP4811(wxNotebook *parent);

    static PageMCP4811 *self;
};
