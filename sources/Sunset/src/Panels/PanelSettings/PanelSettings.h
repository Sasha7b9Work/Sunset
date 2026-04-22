// 2026/04/08 15:14:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panels.h"


class Notebook;


class PanelSettings : public Panel
{
public:

    PanelSettings(Notebook *, PanelSettings *&);

private:

    virtual void CallbackOnEventSize() override;
};

