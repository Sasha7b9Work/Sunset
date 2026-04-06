// 2026/03/19 16:46:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/AnimatedImpulse.h"


AnimatedImpulse::AnimatedImpulse(wxWindow *parent, const wxColor &background) :
    PainterAnimated(parent, wxDefaultPosition, { WIDTH, HEIGHT }),
    color_background(background)
{
    wxWindow::Enable(false);
}


void AnimatedImpulse::FuncDraw()
{
    wxPaintDC _dc(this);

    _dc.SetBrush(wxBrush(color_background));
    _dc.SetPen(wxPen(color_background));

    if(!IsEnabled())
    {
        _dc.DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);
    }
    else
    {
        if (meter.ElapsedMS() > 10)
        {
            x += (int)(2.0f * ((float)meter.ElapsedMS() / 10.0f));
            if (x > GetSize().x)
            {
                x = 0;
            }

            meter.Reset();
        }

        _dc.DrawRectangle(0, 0, GetSize().x - 1, GetSize().y - 1);

        _dc.SetPen(*wxBLACK_PEN);


        int y = 1;

        int y0 = GetSize().y - 2;

        _dc.DrawLine(0, y0, x, y0);
        _dc.DrawLine(x, y0, x, y);
        _dc.DrawLine(x, y, x + 10, y);
        _dc.DrawLine(x + 10, y, x + 10, y0);
        _dc.DrawLine(x + 10, y0, GetSize().x, y0);
    }
}
