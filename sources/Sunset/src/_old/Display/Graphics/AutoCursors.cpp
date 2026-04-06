// 2026/3/15 19:49:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Graphics/AutoCursors.h"
#include "Display/Grid/GridNew.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/FinderMinMax.h"
#include "Panels/PanelConfig/PanelCalc/WindowCursors.h"


AutoCursors *TheAutoCursors = nullptr;


AutoCursors::AutoCursors()
{
    TheAutoCursors = this;
}


void AutoCursors::Draw(const std::vector<GraphMeasure *> &measures)
{
    if (!is_allowed)
    {
        return;
    }

    wxPoint mouse_pos = TheGrid->GetMousePosition();

    wxPoint2DDouble value = TheGrid->CoordToValues(mouse_pos);

    wxRect rect = TheGrid->GetRect();

    TheDisplay->SetColorPen(SET::GUI::color_curve->Get());

    if (TheWindowCursors->chbTrackY->IsChecked())
    {
        FinderMinMax <int>finder(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

        for (GraphMeasure *meas : measures)
        {
            auto result = Math::GetIntersectionY(meas->rel_points, mouse_pos.y);

            if (result.second)
            {
                int x = result.first.x;

                if (Math::InRange(x, rect.GetLeft(), rect.GetRight()))
                {
                    Line(x, rect.GetTop(), x, rect.GetBottom()).Draw();

                    finder.Push(x);
                }
            }
        }

        finder.Push(mouse_pos.x);

        Line(finder.Min(), mouse_pos.y, finder.Max(), mouse_pos.y).Draw(SET::GUI::color_curve->Get());
    }

    if (TheWindowCursors->chbTrackX->IsChecked())
    {
        FinderMinMax <int>finder(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

        for (GraphMeasure *meas : measures)
        {
            auto result = Math::GetIntersectionX(meas->rel_points, mouse_pos.x);

            if (result.second)
            {
                int y = result.first.y;

                if (Math::InRange(y, rect.GetTop(), rect.GetBottom()))
                {
                    Line(rect.GetLeft(), y, rect.GetRight(), y).Draw();
                    finder.Push(y);
                }
            }
        }

        finder.Push(mouse_pos.y);

        Line(mouse_pos.x, finder.Min(), mouse_pos.x, finder.Max()).Draw(SET::GUI::color_curve->Get());
    }

    if (TheWindowCursors->chbTrackMouse->IsChecked())
    {
        Text::SetFont();
        Text(wxString::Format("%.1f : %.1f", value.m_x, -value.m_y)).DrawAboutRightUp(mouse_pos.x + 5, mouse_pos.y - 5, true, true);
    }
}


void AutoCursors::Ban()
{
    is_allowed = false;

    if (TheDisplay)
    {
        TheDisplay->Refresh();
    }
}


void AutoCursors::Allow()
{
    is_allowed = true;
}
