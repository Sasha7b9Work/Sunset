// 2025/7/13 20:38:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "System/Events.h"
#pragma warning(push, 0)
#include <wx/geometry.h>
#pragma warning(pop)


class GraphMeasure;


class IGrid
{
public:

    virtual ~IGrid() {}

    static void Create(IGrid *&);

    virtual void OnChangedOffsetMeasure(const wxPoint &) = 0;
    virtual void OnMouseDown() = 0;
    virtual void OnMouseUp() = 0;
    virtual void MoveImageOn(const wxPoint &) = 0;
    virtual void MoveCenterOn(const wxPoint &) = 0;
    virtual void SetNewMousePosition(const wxPoint &) = 0;
    virtual void ScaleGridOn(const wxPoint &, int) = 0;
    virtual void RangeGridOnX(int) = 0;
    virtual void RangeGridOnY(int) = 0;
    virtual void Draw(const std::vector<GraphMeasure *> &) = 0;
    virtual void ResetCenter() = 0;
    virtual void Reset() = 0;
    virtual wxPoint ValuesToCoord(double x, double y) const = 0;
    virtual int NumCellsX() const = 0;
    virtual int NumCellsY() const = 0;
    virtual wxPoint2DDouble GetRangeX() const = 0;
    virtual wxPoint2DDouble GetRangeY() const = 0;
    virtual wxPoint2DDouble CoordToValues(const wxPoint &) const = 0;
    virtual wxPoint GetMousePosition() const = 0;
    virtual wxRect GetRect() const = 0;
};


class Grid : public IGrid
{
    struct Offset
    {
        // При нажимании/отпускании мышки вызываем эту функцию, чтобы обнулить накопительный счётчик смещения
        void ResetDelta()
        {
            dx = 0;
            dy = 0;
        }

        // При перемещении мышки вызываем эту функцию
        void MoveOn(const wxPoint &delta);

        int x = 0;
        int y = 0;

        wxPoint center_about_screen;    // В этом месте относительно центра экрана находится центр сетки

    private:

        int dx = 0;
        int dy = 0;

        void Process(int &_x, int &_delta);
    };


    struct Range
    {
        Range(const wxString &_title, const wxString &_units, int &_offset) : title(_title), units(_units), value(_offset) {}

        wxString title;
        wxString units;

        // Разница между максимальным и минимальным значениями - амплитуда
        double AmplitudeAbs() const;
        double MaxAbs() const;

        // cells_in_axis - количество клеток по любой оси. Оно всегда одинаковое
        wxString GetValuePointAxis(int, int cells_in_axis) const;

        wxString FullTitle() const;

        void Increase();
        void Decrease();

    private:

        struct Value
        {
            Value(int &_offset) : offset(_offset) {}

            double HalfAmplitudeAbs() const;
            // Минимальное значение
            double MinAbs() const;
            // Максимальное значение
            double MaxAbs() const;
            void Increase();
            void Decrease();

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

    Grid(IGrid *&);

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

    wxPoint2DDouble CoordToValues(const wxPoint &) const override;

    void ResetCenter() override;

    void Reset() override;

    wxPoint2DDouble GetRangeX() const override
    {
        return { 0.0, 0.0 };
    };
    wxPoint2DDouble GetRangeY() const override
    {
        return { 0.0, 0.0 };
    };

    static const int size_cell = 60;       // Столько клетка всегда занимает на экране

private:

    int     scale = 1;              // 2 - увеличено в два раза, 3 - увелично в три и так далее
    wxPoint pos_mouse;
    Offset  offset;
    Range   rangeX{ "Uc", "V", offset.x };
    Range   rangeY{ "Ic", "A", offset.y };

    // d - расстояние между точками
    void DrawVPointLineDown(int x, int y0, int y_low, int d);
    void DrawVPointLineUp(int x, int y0, int y_hi, int d);
    void DrawHPointLineRight(int x, int y, int x_right, int d);
    void DrawHPointLineLeft(int x, int y, int x_left, int d);

    void DrawVPointLineDown2(int x, int y0, int y_low, int d);
    void DrawVPointLineUp2(int x, int y0, int y_hi, int d);
    void DrawHPointLineRight2(int x, int y, int x_right, int d);
    void DrawHPointLineLeft2(int x, int y, int x_left, int d);

    double UnitsInCellX() const;
    double UnitsInCellY() const;

    // Координаты точки оси для подписи значения
    wxPoint GetCoordPointAxisX(int) const;
    wxPoint GetCoordPointAxisY(int) const;

    int BottomY() const;  // В этой позиции экрана находится нижняя сторона экрана
    int TopY() const;     // В этой позиции экрана находится верхняя сторона экрана
    int LeftX() const;    // В этой позиции экрана находится левая сторона сетки
    int RightX() const;   // В этой позиции экрана находится правая сторона сетки

    // Длина оси в пикселях
    int LengthAxis() const;

    void DrawLabelsOnAxis() const;

    // Количество клеток по осям X и Y
    int NumCells() const;

    int NumCellsX() const override
    {
        return NumCells();
    }

    int NumCellsY() const override //-V524
    {
        return NumCells();
    }

    // Отрисовать область сетки, отображаемую на дисплее
    void DrawArea() const;

    // Отобразить окошко навигации (когда сетка целиком не умещается в окне)
    void DrawNavigationWindow() const;

    // Вписать в дисплей таким образом, чтобы не было полей по краям
    void FitIntoDisplay();
};
