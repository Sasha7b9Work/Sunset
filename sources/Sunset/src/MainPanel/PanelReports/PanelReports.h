// 2026/04/08 15:10:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Notebook.h"


class PanelReports : public PageNotebook
{
public:

    PanelReports(Notebook *, PanelReports *&);

private:

    virtual void CallbackOnEventSize() override;
};
