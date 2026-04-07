// 2025/7/13 20:38:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "System/Events.h"
#include "Panels/PanelDisplay/Grid/Grid.h"


class GraphMeasure;


class GridNew : public IGrid
{
    friend class IGrid;

    struct Offset
    {
        void ResetToZero()
        {
            ResetDelta();
            x = 0;
            y = 0;
        }

        // При нажимании/отпускании мышки вызываем эту функцию, чтобы обнулить накопительный счётчик смещения
        void ResetDelta()
        {
            dx = 0;
            dy = 0;
        }

        // При перемещении мышки вызываем эту функцию
        void MoveOn(const wxPoint &delta);

        int &GetX()
        {
            return x;
        }

        int &GetY()
        {
            return y;
        }

        int GetValueX() const
        {
            return x;
        }

        int GetValueY() const
        {
            return y;
        }

    private:

        int x = 0;
        int y = 0;

        int dx = 0;
        int dy = 0;

        void Process(int &_x, int &_delta);
    };


    struct Range
    {
        Range(const wxString &_title, const wxString &_units, int &_offset, int (*num_cells)()) :
            title(_title), units(_units), value(_offset, num_cells) {}

        wxString title;
        wxString units;

        // Разница между максимальным и минимальным значениями - амплитуда
        double AmplitudeAbs() const;
        double MaxAbs() const;
        double MinAbs() const;

        // cells_in_axis - количество клеток по любой оси. Оно всегда одинаковое
        wxString GetValuePointAxis(int, int cells_in_axis) const;

        wxString FullTitle() const;

        void Increase();
        void Decrease();

    private:

        struct Value
        {
            Value(int &_offset, int (*num_cells)()) :
                NumCells(num_cells), offset(_offset) {}

            double HalfAmplitudeAbs() const;
            // Минимальное значение
            double MinAbs() const;
            // Максимальное значение
            double MaxAbs() const;
            void Increase();
            void Decrease();

            int (*NumCells)();

        private:
            // Чему кратно значение - единице, 2, 4(5)
            enum Type
            {
                _1,
                _2,
                _4_5,
                Count
            };

            Type type = _1;
            int order = 0;
            int &offset;        // Смещение 0 относительно центра графика. Измеряется в клетках графика
        };

        Value value;
    };

public:

    void Draw(const std::vector<GraphMeasure *> &) override;

    void MoveImageOn(const wxPoint &) override;     // Переместить изображение на экране целиком
    void MoveCenterOn(const wxPoint &) override;    // Переместить центр графика (избражение измерений в графике)
    void SetNewMousePosition(const wxPoint &) override;

    void OnChangedOffsetMeasure(const wxPoint &) override;

    void OnMouseDown() override;
    void OnMouseUp() override;

    void ScaleGridOn(const wxPoint &, int) override;
    void RangeGridOnX(int) override;
    void RangeGridOnY(int) override;

    // Преобразует точку графика в координаты на холсте
    virtual wxPoint ValuesToCoord(double x, double y) const override;

    wxPoint2DDouble CoordToValues(const wxPoint &) const;

    void ResetCenter() override;

    void Reset() override;

    // Возвращает диапазон, попадающие в который точки нужно отрисовывать
    wxPoint2DDouble GetRangeX() const override;
    wxPoint2DDouble GetRangeY() const override;

    wxRect GetRect() const;
    wxPoint GetMousePosition() const;

    static const int size_cell = 60;       // Столько клетка всегда занимает на экране

private:

    GridNew();

    wxPoint pos_mouse;
    Offset  offset;
    Range   rangeX{ "Uc", "V", offset.GetX(), []()->int
        {
            return TheGrid->NumCellsX();
        } };
    Range   rangeY{ "Ic", "A", offset.GetY(), []() ->int
        {
            return TheGrid->NumCellsY();
        } };

    // d - расстояние между точками
    void DrawVPointLineDown(int x, int y0, int y_low, int d);
    void DrawVPointLineUp(int x, int y0, int y_hi, int d);
    void DrawHPointLineRight(int x, int y, int x_right, int d);
    void DrawHPointLineLeft(int x, int y, int x_left, int d);

    void DrawVPointLineDown2(const wxPoint &, int y_low, int d);
    void DrawVPointLineUp2(const wxPoint &, int y_hi, int d);
    void DrawHPointLineRight2(const wxPoint &, int x_right, int d);
    void DrawHPointLineLeft2(const wxPoint &, int x_left, int d);

    double UnitsInCellX() const;
    double UnitsInCellY() const;

    // Координаты точки оси для подписи значения
    wxPoint GetCoordPointAxisX(int) const;
    wxPoint GetCoordPointAxisY(int) const;

    int BottomY() const;    // В этой позиции экрана находится нижняя сторона сетки
    int TopY() const;       // В этой позиции экрана находится верхняя сторона сетки
    int CenterY() const;    // Середина сетки по Y
    int LeftX() const;      // В этой позиции экрана находится левая сторона сетки
    int RightX() const;     // В этой позиции экрана находится правая сторона сетки
    int CenterX() const;    // Середина сетки по X

    // Координаты точки (0, 0) в пикселях
    wxPoint CoordZeroInPixels() const;

    // Длина оси в пикселях
    int LengthAxisX() const;
    int LengthAxisY() const;

    void DrawLabelsOnAxis() const;

    // Количество клеток по осям X и Y
    int NumCellsX() const override;
    int NumCellsY() const override;
};
