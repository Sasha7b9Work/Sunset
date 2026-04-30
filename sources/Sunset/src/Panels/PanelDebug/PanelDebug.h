// 2025/6/3 13:44:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Notebook.h"


class PanelDebug : public PageNotebook
{
public:

    PanelDebug(Notebook *, PanelDebug *&);

    virtual bool Show(bool) override;

    void PeriodicTask();

    void Unpack();

    void Pack();

    virtual void CallbackOnEventSize() override { }
};
