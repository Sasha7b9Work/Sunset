// 2026/04/08 15:03:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panels.h"


struct PanelBoard;


struct PanelArchive : public Panel
{
    PanelArchive(PanelBoard *);

private:

    virtual void CallbackOnEventSize() override;
};
