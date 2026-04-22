// 2026/04/08 15:14:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Notebook.h"


class PanelSettings : public PageNotebook
{
public:

    PanelSettings(Notebook *, PanelSettings *&);

private:

    virtual void CallbackOnEventSize() override;
};

