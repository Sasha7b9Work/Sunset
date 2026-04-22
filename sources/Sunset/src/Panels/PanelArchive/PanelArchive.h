// 2026/04/08 15:03:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panels.h"


class PanelBoard;


class PanelArchive : public Panel
{
public:

    PanelArchive(PanelBoard *, PanelArchive *&);

private:

    virtual void CallbackOnEventSize() override;
};
