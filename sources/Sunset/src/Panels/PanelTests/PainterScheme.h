// 2025/08/19 12:59:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"


class PainterScheme : public Painter
{
public:
    PainterScheme(wxWindow *parent, const wxSize &size, const wxColor &_colorBackground) :
        Painter(parent, wxDefaultPosition, size),
        colorBackground(_colorBackground)
    {
    }

    // Нарисовать схему в соответствии с установленными настройками
    void Build();

private:

    void DrawElement();

    wxColor colorBackground;
};
