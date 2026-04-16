// 2026/04/08 15:17:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panels.h"


struct PanelBoard;


struct PanelTables : public Panel
{
    PanelTables(PanelBoard *);

private:

    virtual void CallbackOnEventSize() override;
};
