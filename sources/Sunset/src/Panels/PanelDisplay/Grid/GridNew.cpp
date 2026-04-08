// 2025/7/13 20:39:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainWindow.h"
#include "Utils/Math.h"
#include "Panels/PanelDisplay/Graphics/GraphMeasure.h"
#include "Panels/PanelDisplay/Grid/GridNew.h"
#include "Panels/PanelDisplay/PanelMeasures.h"
#include "Settings/Settings.h"
#include "Panels/PanelDisplay/Graphics/AutoCursors.h"


GridNew::GridNew()
{
    Reset();
}


void GridNew::ResetCenter()
{
}


void GridNew::Reset()
{
    offset.ResetToZero();
}


int GridNew::BottomY() const
{
    return CenterY() + LengthAxisY() / 2;
}


int GridNew::TopY() const
{
    return CenterY() - LengthAxisY() / 2;
}


int GridNew::CenterY() const
{
    return ThePanelMeasures->GetDrawingSize().y / 2;
}


int GridNew::CenterX() const
{
    return ThePanelMeasures->GetDrawingSize().x / 2;
}


int GridNew::LeftX() const
{
    return CenterX() - LengthAxisX() / 2;
}


int GridNew::RightX() const
{
    return CenterX() + LengthAxisX() / 2;
}


int GridNew::LengthAxisX() const
{
    return size_cell * NumCellsX();
}


int GridNew::LengthAxisY() const
{
    return size_cell * NumCellsY();
}


void GridNew::Draw(const std::vector<GraphMeasure *> &entities)
{
    wxSize size = ThePanelMeasures->GetDrawingSize();

    const int x_left = LeftX();
    const int x_right = RightX();
    const int y_top = TopY();
    const int y_bottom = BottomY();

    int d = 5;

    const wxPoint coord_zero = CoordZeroInPixels();

    {
        // Горизонтальные линии
        Line(x_left, y_top, RightX(), y_top).Draw(SET::GUI::color_grid->Get());

        DrawHPointLineRight2(coord_zero, size.x, d);
        DrawHPointLineLeft2(coord_zero, 0, d);

        Line(x_left, BottomY(), RightX(), BottomY()).Draw();

        // Вертикальные линии
        Line(x_left, y_top, x_left, BottomY()).Draw();

        DrawVPointLineDown2(coord_zero, size.y, d);
        DrawVPointLineUp2(coord_zero, 0, d);

        Line(RightX(), y_top, RightX(), BottomY()).Draw();
    }

    // Рисуем вертикальные линии справа от нуля
    for (int i = 1; i < 100; i++)
    {
        int x = coord_zero.x + i * size_cell;

        if (x > 0 && x < size.x)
        {
            DrawVPointLineDown(x, coord_zero.y, size.y, d);
            DrawVPointLineUp(x, coord_zero.y, 0, d);
        }
    }

    // Рисуем вертикальные линии слева от нуля
    for (int i = 1; i < 100; i++)
    {
        int x = coord_zero.x - i * size_cell;

        if (x > 0 && x < size.x)
        {
            DrawVPointLineDown(x, coord_zero.y, size.y, d);
            DrawVPointLineUp(x, coord_zero.y, 0, d);
        }
    }

    // Рисуем горизонтальные линии сверху от нуля
    for (int i = 1; i < 100; i++)
    {
        int y = coord_zero.y - i * size_cell;

        if (y > 0 && y < size.y)
        {
            DrawHPointLineRight(coord_zero.x, y, size.x, d);
            DrawHPointLineLeft(coord_zero.x, y, 0, d);
        }
    }

    // Рисуем горизонтальные линии снизу от нуля
    for (int i = 1; i < 100; i++)
    {
        int y = coord_zero.y + i * size_cell;

        if (y > 0 && y < size.y)
        {
            DrawHPointLineRight(coord_zero.x, y, size.x, d);
            DrawHPointLineLeft(coord_zero.x, y, 0, d);
        }
    }

    d = size_cell / 5;

    for (int i = 1; i < 3; i++)
    {
        DrawVPointLineUp(x_left + i, coord_zero.y, 0, d);
        DrawVPointLineDown(x_left + i, coord_zero.y, size.y, d);

        DrawHPointLineRight(coord_zero.x, BottomY() - i, size.x, d);
        DrawHPointLineLeft(coord_zero.x, BottomY() - i, 0, d);
    }

    for (auto *entity : entities)
    {
        entity->Draw();
    }

    ThePanelMeasures->SetColorPen(SET::GUI::color_background->Get());

    ThePanelMeasures->FillRectangle(0, 0, x_left - 1, ThePanelMeasures->GetDrawingSize().y, SET::GUI::color_background->Get()); //-V807
    ThePanelMeasures->FillRectangle(x_left, 0, LengthAxisX(), y_top - 1, SET::GUI::color_background->Get());
    ThePanelMeasures->FillRectangle(x_right + 1, 0, ThePanelMeasures->GetDrawingSize().x - x_right, ThePanelMeasures->GetDrawingSize().y, SET::GUI::color_background->Get());
    ThePanelMeasures->FillRectangle(x_left, y_bottom + 1, LengthAxisX(), ThePanelMeasures->GetDrawingSize().y - y_bottom, SET::GUI::color_background->Get());

    if (!ThePanelMeasures->mouse_is_pressed)
    {
        if (pos_mouse.y > TopY() &&
            pos_mouse.y < BottomY() &&
            pos_mouse.x > LeftX() &&
            pos_mouse.x < RightX())
        {
            TheAutoCursors->Draw(entities);
        }
    }

    DrawLabelsOnAxis();
}


void GridNew::DrawLabelsOnAxis() const
{
    ThePanelMeasures->SetColorPen(SET::GUI::color_font->Get());

    Text::SetFont();

    int d = 2;

    wxSize size = ThePanelMeasures->GetDrawingSize();

    {
        // Подписываем горизонтальную ось

        wxPoint last_pos{ 0, 0 };   // Здесь отрисовано последнее значение

        // Значения
        for (int i = -100; i < 100; i++)
        {
            wxPoint coord = GetCoordPointAxisX(i);

            // Если влазит под сеткой
            if (BottomY() + 20 < size.y)
            {
                if (Math::InRange(coord.x, LeftX() + 1, RightX()))
                {
                    last_pos = { coord.x, coord.y + d };

                    wxString value = rangeX.GetValuePointAxis(i, NumCellsX());

                    Text(value).DrawAboutCenterDown(last_pos.x, last_pos.y, true);
                }
            }
            else
            {
                if (Math::InRange(coord.x, LeftX() + 1, RightX()))
                {
                    last_pos = { coord.x, size.y - 25 };
                    Text(rangeX.GetValuePointAxis(i, NumCellsX())).DrawAboutCenterDown(last_pos.x, last_pos.y, true);
                }
            }
        }

        Text(rangeX.FullTitle()).DrawAboutCenterDown(last_pos.x < size.x ? last_pos.x : size.x - 20, last_pos.y - 20, true);
    }

    {
        // Подписываем вертикальную ось

        wxPoint last_pos{ 0, 0 };   // Здесь отрисовано последнее значение

        for (int i = -100; i < 100; i++)
        {
            wxPoint coord = GetCoordPointAxisY(-i);

            // Если влазит слева от сетки
            if (LeftX() - 20 > 0)
            {
                if (Math::InRange(coord.y, TopY(), BottomY() - 1))
                {
                    last_pos = { coord.x - d, coord.y };
                    Text(rangeY.GetValuePointAxis(i, NumCellsY())).DrawAboutCenterLeft(last_pos.x, last_pos.y, true);
                }
            }
            else
            {
                if (Math::InRange(coord.y, TopY(), BottomY() - 1))
                {
                    last_pos = { d, coord.y };
                    Text(rangeY.GetValuePointAxis(i, NumCellsY())).DrawAboutCenterRigth(last_pos.x, last_pos.y, true);
                }
            }
        }

        d = 7;

        // Единицы измерения
        if (LeftX() - 20 > 0)
        {
            Text(rangeY.FullTitle()).DrawAboutCenterLeft(last_pos.x + 40, last_pos.y < d ? d : last_pos.y, true);
        }
        else
        {
            Text(rangeY.FullTitle()).DrawAboutCenterRigth(last_pos.x + 40, last_pos.y < d ? d : last_pos.y, true);
        }
    }
}


wxPoint GridNew::GetCoordPointAxisX(int num) const
{
    return { CoordZeroInPixels().x + size_cell * num, BottomY() };
}


wxPoint GridNew::GetCoordPointAxisY(int num) const
{
    return { LeftX(), CoordZeroInPixels().y + size_cell * num };
}


void GridNew::SetNewMousePosition(const wxPoint &position)
{
    pos_mouse = position;
}



void GridNew::MoveCenterOn(const wxPoint &delta)
{
    offset.MoveOn(delta);
}


void GridNew::MoveImageOn(const wxPoint &)
{
}


void GridNew::OnChangedOffsetMeasure(const wxPoint &)
{
}


void GridNew::ScaleGridOn(const wxPoint &, int)
{
}


void GridNew::RangeGridOnX(int delta)
{
    if (delta < 0)
    {
        rangeX.Increase();
    }
    else
    {
        rangeX.Decrease();
    }

    ThePanelMeasures->Refresh();
}


void GridNew::RangeGridOnY(int delta)
{
    if (delta < 0)
    {
        rangeY.Increase();
    }
    else
    {
        rangeY.Decrease();
    }

    ThePanelMeasures->Refresh();
}


void GridNew::DrawVPointLineDown(int x, int y0, int y_low, int d)
{
    for (int i = y0; i < y_low; i += d)
    {
        Point().Draw(x, i);
    }
}


void GridNew::DrawVPointLineUp(int x, int y0, int y_hi, int d)
{
    for (int i = y0; i > y_hi; i -= d)
    {
        Point().Draw(x, i);
    }
}


void GridNew::DrawHPointLineRight(int x, int y, int x_right, int d)
{
    for (int i = x; i < x_right; i += d)
    {
        Point().Draw(i, y);
    }
}


void GridNew::DrawHPointLineLeft(int x, int y, int x_left, int d)
{
    for (int i = x; i > x_left; i -= d)
    {
        Point().Draw(i, y);
    }
}


void GridNew::DrawVPointLineDown2(const wxPoint &p, int y_low, int d)
{
    int x = p.x;
    int y = p.y;

    for (int i = y; i < y_low; i += d)
    {
        Point().Draw(x, i);
        Point().Draw(x, i + 1);
        Point().Draw(x, i + 2);
    }
}


void GridNew::DrawVPointLineUp2(const wxPoint &p, int y_hi, int d)
{
    int x = p.x;
    int y = p.y;

    for (int i = y; i > y_hi; i -= d)
    {
        Point().Draw(x, i);
        Point().Draw(x, i - 1);
        Point().Draw(x, i - 2);
    }
}


void GridNew::DrawHPointLineRight2(const wxPoint &p, int x_right, int d)
{
    int x = p.x;
    int y = p.y;

    for (int i = x; i < x_right; i += d)
    {
        Point().Draw(i, y);
        Point().Draw(i + 1, y);
        Point().Draw(i + 2, y);
    }
}


void GridNew::DrawHPointLineLeft2(const wxPoint &p, int x_left, int d)
{
    int x = p.x;
    int y = p.y;

    for (int i = x; i > x_left; i -= d)
    {
        Point().Draw(i, y);
        Point().Draw(i - 1, y);
        Point().Draw(i - 2, y);
    }
}


double GridNew::UnitsInCellX() const
{
    return rangeX.AmplitudeAbs() / NumCellsX();
}


double GridNew::UnitsInCellY() const
{
    return rangeY.AmplitudeAbs() / NumCellsY();
}


int GridNew::NumCellsX() const
{
    if (ModeMainWindow::Current() == ModeMainWindow::FullGraph)
    {
        return 16;
    }

    return 12;
}


int GridNew::NumCellsY() const
{
    if (ModeMainWindow::Current() == ModeMainWindow::FullGraph)
    {
        return 12;
    }

    return 10;
}


wxPoint GridNew::ValuesToCoord(double x, double y) const
{
    double cells_in_x = x * NumCellsX() / rangeX.AmplitudeAbs();

    double cells_in_y = y * NumCellsY() / rangeY.AmplitudeAbs();

    wxPoint coord_zero = CoordZeroInPixels();

    return { (int)(coord_zero.x + cells_in_x * size_cell + 0.5), (int)(coord_zero.y - cells_in_y * size_cell + 0.5) };
}


wxPoint2DDouble GridNew::CoordToValues(const wxPoint &coord) const
{
    wxPoint coord_zero = CoordZeroInPixels();

    return {
        rangeX.AmplitudeAbs() * (coord.x - coord_zero.x) / (NumCellsX() * size_cell),
        rangeY.AmplitudeAbs() * (coord.y - coord_zero.y) / (NumCellsY() * size_cell)
    };
}


void GridNew::OnMouseDown()
{
    offset.ResetDelta();
}


void GridNew::OnMouseUp()
{
    offset.ResetDelta();
}


double GridNew::Range::AmplitudeAbs() const
{
    return value.HalfAmplitudeAbs() * 2.0;
}


double GridNew::Range::MaxAbs() const
{
    return value.MaxAbs();
}


double GridNew::Range::MinAbs() const
{
    return value.MinAbs();
}


void GridNew::Range::Increase()
{
    value.Increase();
}


void GridNew::Range::Decrease()
{
    value.Decrease();
}


double GridNew::Range::Value::HalfAmplitudeAbs() const
{
    static double values[Type::Count] =
    {
        1.0,
        2.0,
        5.0
    };

    double result = values[type];

    if (order > 0)
    {
        result = 1.0;

        for (int i = 0; i < order; i++)
        {
            result *= 10.0;
        }

        result *= values[type];
    }
    else if(order < 0)
    {
        result = 1.0;

        for (int i = order; i < 0; i++)
        {
            result *= 0.1;
        }

        result *= values[type];
    }

    return result * NumCells() / 10.0;
}


double GridNew::Range::Value::MaxAbs() const
{
    double max = HalfAmplitudeAbs();

    return max + (double)offset * HalfAmplitudeAbs() / (NumCells() / 2.0);
}


double GridNew::Range::Value::MinAbs() const
{
    double min = -HalfAmplitudeAbs();

    return min + (double)offset * HalfAmplitudeAbs() / (NumCells() / 2.0);
}


void GridNew::Range::Value::Increase()
{
    if ((MaxAbs() - MinAbs()) > 3e3)
    {
        return;
    }

    type = (Type)((int)type + 1);
    if (type == Count)
    {
        type = (Type)0;
        order++;
    }
}


void GridNew::Range::Value::Decrease()
{
    if ((MaxAbs() - MinAbs()) < 1e-11)
    {
        return;
    }

    int new_type = (int)type - 1;

    if (new_type < 0)
    {
        new_type = Count - 1;
        order--;
    }

    type = (Type)new_type;
}


wxString GridNew::Range::FullTitle() const
{
    wxString prefix;

    double max_abs = MaxAbs();

    if (max_abs > 1e3)
    {
        prefix = "k";
    }
    else if (max_abs > 1.0)
    {
        prefix = "";
    }
    else if ((int64)(max_abs * 1000) > 1)
    {
        prefix = "m";
    }
    else if ((int64)(max_abs * 1000000) > 1)
    {
        prefix = "u";
    }
    else if ((int64)(max_abs * 1e9) > 1)
    {
        prefix = "n";
    }
    else if ((int64)(max_abs * 1e12) > 1)
    {
        prefix = "p";
    }

    return title + "," + prefix + units;
}


wxString GridNew::Range::GetValuePointAxis(int num, int cells_in_axis) const
{
    double step = AmplitudeAbs() / cells_in_axis;

    double max_abs = MaxAbs();

    if (max_abs > 1e3)
    {
        step /= 1e3;
    }
    else if (max_abs > 1)
    {
        step *= 1.0;
    }
    else if ((int64)(max_abs * 1000) > 1)
    {
        step *= 1e3;
    }
    else if ((int64)(max_abs * 1e6) > 1)
    {
        step *= 1e6;
    }
    else if ((int64)(max_abs * 1e9) > 1)
    {
        step *= 1e9;
    }
    else if ((int64)(max_abs * 1e12) > 1)
    {
        step *= 1e12;
    }

    double val = step * num;

    return wxString::Format("%.1f", val);
}


void GridNew::Offset::Process(int &_value, int &_delta)
{
    int d = GridNew::size_cell;

    while (_delta >= d)
    {
        _delta -= d;
        _value++;
    }

    while (_delta <= -d)
    {
        _delta += d;
        _value--;
    }
}


void GridNew::Offset::MoveOn(const wxPoint &delta)
{
    dx += delta.x;
    dy += delta.y;

    Process(x, dx);
    Process(y, dy);
}


wxPoint GridNew::CoordZeroInPixels() const
{
    return { CenterX() + offset.GetValueX() * size_cell, CenterY() + offset.GetValueY() * size_cell };
}


wxPoint2DDouble GridNew::GetRangeX() const
{
    double min = rangeX.MinAbs() - rangeX.AmplitudeAbs() * 1e1;
    double max = rangeX.MaxAbs() + rangeX.AmplitudeAbs() * 1e1;

    return { min, max };
}


wxPoint2DDouble GridNew::GetRangeY() const
{
    double min = rangeY.MinAbs() - rangeY.AmplitudeAbs() * 1e1;
    double max = rangeY.MaxAbs() + rangeY.AmplitudeAbs() * 1e1;

    return { min, max };
}


wxRect GridNew::GetRect() const
{
    int x = LeftX();
    int y = TopY();
    int w = RightX() - LeftX();
    int h = BottomY() - TopY();

    return { x, y, w, h };
}


wxPoint GridNew::GetMousePosition() const
{
    return pos_mouse;
}
