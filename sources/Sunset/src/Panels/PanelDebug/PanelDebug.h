// 2025/6/3 13:44:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panels.h"


struct PanelDebug : public Panel
{
    PanelDebug(wxWindow *);

    virtual bool Show(bool) override;

    void PeriodicTask();

    void Unpack();

    void Pack();

private:

    virtual void CallbackOnEventSize() override;
};
