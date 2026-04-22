// 2026/04/08 15:10:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panels.h"


class Notebook;


class PanelReports : public Panel
{
public:

    PanelReports(Notebook *, PanelReports *&);

private:

    virtual void CallbackOnEventSize() override;
};
