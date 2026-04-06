// 2025/6/1 18:24:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/Panel.h"
#include "Controls/Buttons.h"


class PanelIndicator : public Panel
{
public:

    static const int HEIGHT = 64;

    PanelIndicator(wxWindow *parent);

    static PanelIndicator *self;

private:

    ButtonBitmapChoice *button_start = nullptr;

    void OnEventButton(ButtonBitmapChoiceEvent &);
};
