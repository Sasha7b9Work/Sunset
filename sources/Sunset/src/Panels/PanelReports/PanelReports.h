// 2026/04/08 15:10:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panels.h"


struct PanelBoard;


struct PanelReports : public Panel
{
    PanelReports(PanelBoard *);

private:

    virtual void CallbackOnEventSize() override;
};
