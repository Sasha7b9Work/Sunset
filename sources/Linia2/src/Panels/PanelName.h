// 2025/6/1 16:28:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"


class PanelName : public Panel
{
public:
    PanelName(wxWindow *parent);

    static PanelName *self;

private:

    wxStaticText *textDate = nullptr;
    wxStaticText *textTime = nullptr;

    wxTimer timer;

    void OnTimer(wxTimerEvent &);

    void WriteDateTime();
};
