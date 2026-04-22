// 2026/04/08 15:17:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Notebook.h"


class PanelTables : public Panel
{
public:

    PanelTables(Notebook *, PanelTables *&);

private:

    virtual void CallbackOnEventSize() override;
};
