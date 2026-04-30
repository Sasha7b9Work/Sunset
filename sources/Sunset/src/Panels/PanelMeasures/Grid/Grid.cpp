// 2025/7/13 20:39:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainWindow.h"
#include "Utils/Math.h"
#include "Panels/PanelMeasures/Graphics/GraphMeasure.h"
#include "Panels/PanelMeasures/Grid/Grid.h"
#include "Panels/PanelMeasures/Grid/GridNew.h"
#include "Panels/PanelMeasures/PanelMeasures.h"
#include "Panels/PanelMeasures/Graphics/AutoCursors.h"
#include "Settings/Settings.h"


IGrid *TheGrid = nullptr;


void IGrid::Create()
{
    if (!TheGrid)
    {
        TheGrid = new GridNew(TheGrid);
    }
}


Grid::Grid(IGrid *&self)
{
    self = this;

    Reset();
}


void Grid::ResetCenter()
{
    wxSize display_size = ThePanelMeasures->GetDrawingSize();

    offset.center_about_screen = { display_size.x / 2, display_size.y / 2 };
}


void Grid::Reset()
{
    scale = 1;

    ResetCenter();
}


int Grid::BottomY() const
{
    return TopY() + LengthAxis();
}


int Grid::TopY() const
{
    return offset.center_about_screen.y - (int)(rangeY.MaxAbs() / UnitsInCellY() * size_cell);
}


int Grid::LengthAxis() const
{
    return size_cell * NumCells();
}


int Grid::LeftX() const
{
    return offset.center_about_screen.x - (int)(rangeX.MaxAbs() / UnitsInCellX() * size_cell);
}


int Grid::RightX() const
{
    return LeftX() + LengthAxis();
}


void Grid::DrawArea() const
{
    if (scale == 1)
    {
        return;
    }

    wxSize size = ThePanelMeasures->GetDrawingSize();

    int size_x = size.x * size.x / LengthAxis();
    int size_y = size.y * size.y / LengthAxis();

    int d_x = -LeftX() * size.x / LengthAxis();
    int d_y = -TopY() * size.y / LengthAxis();

    wxColor color{ 127, 127, 127 };

    Rect(size_x, 2).Fill(d_x, 0, color);
    Rect(2, size_y).Fill(size.x - 4, d_y, color);
}


void Grid::DrawNavigationWindow() const
{
    if (scale == 1 || !wxGetMouseState().LeftIsDown())
    {
        return;
    }

    wxSize size_window { 150, 150 };

    ThePanelMeasures->FillRectangle(0, 0, size_window.x, size_window.y, { 240, 240, 240 });

    {
        wxSize size = ThePanelMeasures->GetDrawingSize();

        int size_x = size.x * size_window.x / LengthAxis();
        int size_y = size.y * size_window.y / LengthAxis();

        int dx = -LeftX() * size_window.x / LengthAxis();
        int dy = -TopY() * size_window.y / LengthAxis();

        Rect(size_x, size_y).Fill(dx, dy, *wxWHITE);
    }

    Rect(size_window.x, size_window.y).Draw(0, 0, *wxBLACK);
}


void Grid::Draw(const std::vector<GraphMeasure *> &entities)
{
    wxSize size = ThePanelMeasures->GetDrawingSize();

    const int length = LengthAxis();

    const int x_left = LeftX();
    const int x_right = RightX();
    const int y_top = TopY();
    const int y_bottom = BottomY();

    int d = 5;

    {
        // Горизонтальные линии
        Line(x_left, y_top, RightX(), y_top).Draw(*wxBLACK);

        if (Math::InRange(offset.center_about_screen.y, y_top, y_bottom))
        {
            DrawHPointLineRight2(offset.center_about_screen.x, offset.center_about_screen.y, size.x, d);
            DrawHPointLineLeft2(offset.center_about_screen.x, offset.center_about_screen.y, 0, d);
        }

        Line(x_left, BottomY(), RightX(), BottomY()).Draw();

        // Вертикальные линии
        Line(x_left, y_top, x_left, BottomY()).Draw();

        if (Math::InRange(offset.center_about_screen.x, x_left, x_right))
        {
            DrawVPointLineDown2(offset.center_about_screen.x, offset.center_about_screen.y, size.y, d);
            DrawVPointLineUp2(offset.center_about_screen.x, offset.center_about_screen.y, 0, d);
        }

        Line(RightX(), y_top, RightX(), BottomY()).Draw();
    }

    // Рисуем вертикальные линии справа от нуля
    for (int i = 1; i < 100; i++)
    {
        int x = offset.center_about_screen.x + i * size_cell;

        if (x > 0 && x < size.x)
        {
            DrawVPointLineDown(x, offset.center_about_screen.y, size.y, d);
            DrawVPointLineUp(x, offset.center_about_screen.y, 0, d);
        }
    }

    // Рисуем вертикальные линии слева от нуля
    for (int i = 1; i < 100; i++)
    {
        int x = offset.center_about_screen.x - i * size_cell;

        if (x > 0 && x < size.x)
        {
            DrawVPointLineDown(x, offset.center_about_screen.y, size.y, d);
            DrawVPointLineUp(x, offset.center_about_screen.y, 0, d);
        }
    }

    // Рисуем горизонтальные линии сверху от нуля
    for (int i = 1; i < 100; i++)
    {
        int y = offset.center_about_screen.y - i * size_cell;

        if (y > 0 && y < size.y)
        {
            DrawHPointLineRight(offset.center_about_screen.x, y, size.x, d);
            DrawHPointLineLeft(offset.center_about_screen.x, y, 0, d);
        }
    }

    // Рисуем горизонтальные линии снизу от нуля
    for (int i = 1; i < 100; i++)
    {
        int y = offset.center_about_screen.y + i * size_cell;

        if (y > 0 && y < size.y)
        {
            DrawHPointLineRight(offset.center_about_screen.x, y, size.x, d);
            DrawHPointLineLeft(offset.center_about_screen.x, y, 0, d);
        }
    }

    d = size_cell / 5;

    for (int i = 1; i < 3; i++)
    {
        DrawVPointLineUp(x_left + i, offset.center_about_screen.y, 0, d);
        DrawVPointLineDown(x_left + i, offset.center_about_screen.y, size.y, d);

        DrawHPointLineRight(offset.center_about_screen.x, BottomY() - i, size.x, d);
        DrawHPointLineLeft(offset.center_about_screen.x, BottomY() - i, 0, d);
    }

    for (auto *entity : entities)
    {
        entity->Draw();
    }

    DrawArea();

    if (scale == 1)
    {
        ThePanelMeasures->FillRectangle(0, 0, x_left - 1, ThePanelMeasures->GetDrawingSize().y, *wxWHITE); //-V807
        ThePanelMeasures->FillRectangle(x_left, 0, length, y_top - 1, *wxWHITE);
        ThePanelMeasures->FillRectangle(x_right + 1, 0, ThePanelMeasures->GetDrawingSize().x - x_right, ThePanelMeasures->GetDrawingSize().y, *wxWHITE);
        ThePanelMeasures->FillRectangle(x_left, y_bottom + 1, length, ThePanelMeasures->GetDrawingSize().y - y_bottom, *wxWHITE);
    }

    DrawLabelsOnAxis();

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

    DrawNavigationWindow();
}


void Grid::DrawLabelsOnAxis() const
{
//    wxColor background{ 230, 230, 230 };

    Text::SetFont();

    int d = 2;

    wxSize size = ThePanelMeasures->GetDrawingSize();

    {
        // Подписываем горизонтульную ось

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
                    Text(rangeX.GetValuePointAxis(i, NumCells())).DrawAboutCenterDown(last_pos.x, last_pos.y, true);
                }
            }
            else
            {
                if (Math::InRange(coord.x, LeftX() + 1, RightX()))
                {
                    last_pos = { coord.x, size.y - 25 };
                    Text(rangeX.GetValuePointAxis(i, NumCells())).DrawAboutCenterDown(last_pos.x, last_pos.y, true);
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
                    Text(rangeY.GetValuePointAxis(i, NumCells())).DrawAboutCenterLeft(last_pos.x, last_pos.y, true);
                }
            }
            else
            {
                if (Math::InRange(coord.y, TopY(), BottomY() - 1))
                {
                    last_pos = { d, coord.y };
                    Text(rangeY.GetValuePointAxis(i, NumCells())).DrawAboutCenterRigth(last_pos.x, last_pos.y, true);
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


wxPoint Grid::GetCoordPointAxisX(int num) const
{
    return { offset.center_about_screen.x + size_cell * num, BottomY() };
}


wxPoint Grid::GetCoordPointAxisY(int num) const
{
    return { LeftX(), offset.center_about_screen.y + size_cell * num };
}


void Grid::SetNewMousePosition(const wxPoint &position)
{
    pos_mouse = position;
}



void Grid::MoveCenterOn(const wxPoint &delta)
{
    offset.MoveOn(delta);
}


void Grid::MoveImageOn(const wxPoint &delta)
{
    if (scale == 1)
    {
        return;
    }

    offset.center_about_screen += delta * scale;

    FitIntoDisplay();
}


void Grid::OnChangedOffsetMeasure(const wxPoint &delta)
{
    offset.center_about_screen += delta * size_cell;
}


void Grid::FitIntoDisplay()
{
    if (scale == 1)
    {
        return;
    }

    wxSize size = ThePanelMeasures->GetDrawingSize();

    if (LeftX() > 5)
    {
        offset.center_about_screen.x = 5 + LengthAxis() / 2;
    }

    if (TopY() > 5)
    {
        offset.center_about_screen.y = 5 + LengthAxis() / 2;
    }

    int d = 5;

    if (RightX() < size.x - d)
    {
        offset.center_about_screen.x = size.x - d - LengthAxis() / 2;
    }

    if (BottomY() < size.y - d)
    {
        offset.center_about_screen.y = size.y - d - LengthAxis() / 2;
    }
}


void Grid::ScaleGridOn(const wxPoint &pos, int delta)
{
    wxPoint delta_center = offset.center_about_screen - pos;

    if (delta > 0 && scale < 8)
    {
        scale *= 2;

        offset.center_about_screen += delta_center;
    }
    else if (delta < 0 && scale > 1)
    {
        scale /= 2;

        offset.center_about_screen -= delta_center / 2;
    }

    if (scale == 1)
    {
        ResetCenter();
    }

    FitIntoDisplay();
}


void Grid::RangeGridOnX(int delta)
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


void Grid::RangeGridOnY(int delta)
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


void Grid::DrawVPointLineDown(int x, int y0, int y_low, int d)
{
    for (int i = y0; i < y_low; i += d)
    {
        Point().Draw(x, i);
    }
}


void Grid::DrawVPointLineUp(int x, int y0, int y_hi, int d)
{
    for (int i = y0; i > y_hi; i -= d)
    {
        Point().Draw(x, i);
    }
}


void Grid::DrawHPointLineRight(int x, int y, int x_right, int d)
{
    for (int i = x; i < x_right; i += d)
    {
        Point().Draw(i, y);
    }
}


void Grid::DrawHPointLineLeft(int x, int y, int x_left, int d)
{
    for (int i = x; i > x_left; i -= d)
    {
        Point().Draw(i, y);
    }
}


void Grid::DrawVPointLineDown2(int x, int y0, int y_low, int d)
{
    for (int i = y0; i < y_low; i += d)
    {
        Point().Draw(x, i);
        Point().Draw(x, i + 1);
        Point().Draw(x, i + 2);
    }
}


void Grid::DrawVPointLineUp2(int x, int y0, int y_hi, int d)
{
    for (int i = y0; i > y_hi; i -= d)
    {
        Point().Draw(x, i);
        Point().Draw(x, i - 1);
        Point().Draw(x, i - 2);
    }
}


void Grid::DrawHPointLineRight2(int x, int y, int x_right, int d)
{
    for (int i = x; i < x_right; i += d)
    {
        Point().Draw(i, y);
        Point().Draw(i + 1, y);
        Point().Draw(i + 2, y);
    }
}


void Grid::DrawHPointLineLeft2(int x, int y, int x_left, int d)
{
    for (int i = x; i > x_left; i -= d)
    {
        Point().Draw(i, y);
        Point().Draw(i - 1, y);
        Point().Draw(i - 2, y);
    }
}


double Grid::UnitsInCellX() const
{
    return rangeX.AmplitudeAbs() / NumCells();
}


double Grid::UnitsInCellY() const
{
    return rangeY.AmplitudeAbs() / NumCells();
}


int Grid::NumCells() const
{
    return 10 * scale;
}


wxPoint Grid::ValuesToCoord(double x, double y) const
{
    double cells_in_x = x * NumCells() / rangeX.AmplitudeAbs();

    double cells_in_y = y * NumCells() / rangeY.AmplitudeAbs();

    return { (int)(offset.center_about_screen.x + cells_in_x * size_cell + 0.5), (int)(offset.center_about_screen.y - cells_in_y * size_cell + 0.5) };
}


wxPoint2DDouble Grid::CoordToValues(const wxPoint &coord) const
{
    return {
        rangeX.AmplitudeAbs() * (coord.x - offset.center_about_screen.x) / (NumCells() * size_cell),
        rangeY.AmplitudeAbs() * (coord.y - offset.center_about_screen.y) / (NumCells() * size_cell)
    };
}


void Grid::OnMouseDown()
{
    offset.ResetDelta();
}


void Grid::OnMouseUp()
{
    offset.ResetDelta();
}


double Grid::Range::AmplitudeAbs() const
{
    return value.HalfAmplitudeAbs() * 2.0;
}


double Grid::Range::MaxAbs() const
{
    return value.MaxAbs();
}


void Grid::Range::Increase()
{
    value.Increase();
}


void Grid::Range::Decrease()
{
    value.Decrease();
}


double Grid::Range::Value::HalfAmplitudeAbs() const
{
    static double values[Type::Count] =
    {
        1.0,
        2.0,
        5.0
    };

    if (order == 0)
    {
        return values[type];
    }
    else if (order > 0)
    {
        double result = 1.0;
        for (int i = 0; i < order; i++)
        {
            result *= 10.0;
        }
        return result * values[type];
    }
    else
    {
        double result = 1.0;
        for (int i = order; i < 0; i++)
        {
            result *= 0.1;
        }
        return result * values[type];
    }
}


double Grid::Range::Value::MaxAbs() const
{
    double max = HalfAmplitudeAbs();

    return max + (double)offset * HalfAmplitudeAbs() / 5.0;
}


double Grid::Range::Value::MinAbs() const
{
    double min = -HalfAmplitudeAbs();

    return min + (double)offset * HalfAmplitudeAbs() / 5.0f;
}


void Grid::Range::Value::Increase()
{
    if (MaxAbs() > 3e3)
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


void Grid::Range::Value::Decrease()
{
    if (MaxAbs() < 1e-11)
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


wxString Grid::Range::FullTitle() const
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
    else if((int64)(max_abs * 1000) > 1)
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


wxString Grid::Range::GetValuePointAxis(int num, int cells_in_axis) const
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

    return wxString::Format("%.1f", step * num);
}


void Grid::Offset::Process(int &_value, int &_delta)
{
    int d = Grid::size_cell;

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


void Grid::Offset::MoveOn(const wxPoint &delta)
{
    dx += delta.x;
    dy += delta.y;

    wxPoint diff{ x, y };

    Process(x, dx);
    Process(y, dy);

    if (x != diff.x || y != diff.y)
    {
        diff.x = x - diff.x;
        diff.y = y - diff.y;

        Events::ChangeOffsetMeasure(diff);
    }
}
