// 2026/04/08 15:03:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Notebook.h"


class PanelArchive : public Panel
{
public:

    PanelArchive(Notebook *, PanelArchive *&);

private:

    virtual void CallbackOnEventSize() override;
};
