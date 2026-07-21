// 2025/10/07 11:52:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/Notebook/PageChip.h"


/*

    Канал C - Формирователь

*/

class PageChannelForm : public PageChip
{
public:

    PageChannelForm(wxNotebook *);

    static PageChannelForm *self;
};
