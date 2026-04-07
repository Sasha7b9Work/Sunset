// 2025/6/3 13:44:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelDebug : public Panel
{
public:

    PanelDebug(wxWindow *parent);

    virtual bool Show(bool) override;

    void PeriodicTask();

    void Unpack();

    void Pack();
};
