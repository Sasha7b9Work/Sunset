// 2026/03/14 01:12:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Класс для интерполяции кубическими сплайнами
class CubicSpline {
private:
    std::vector<double> x;      // исходные x-координаты
    std::vector<double> y;      // исходные y-координаты
    std::vector<double> a, b, vec_c, vec_d; // коэффициенты сплайна
    bool valid;                 // флаг успешного построения

public:
    CubicSpline();

    // Инициализация сплайна по точкам
    // boundary: 0 - натуральный (вторая производная = 0), 
    //           1 - закрепленный (нужно указать производные на концах)
    bool Initialize(const std::vector<wxPoint> &points,
        int boundary = 0,
        double leftDerivative = 0,
        double rightDerivative = 0);

    bool Initialize(const std::vector<wxRealPoint> &points,
        int boundary = 0,
        double leftDerivative = 0,
        double rightDerivative = 0);

    // Вычисление значения сплайна в произвольной точке t
    double Evaluate(double t) const;

    // Получение всех точек для отрисовки с заданным шагом
    std::vector<wxPoint> GetPoints(double step = 1.0) const;

    bool IsValid() const
    {
        return valid;
    }

private:
    // Решение трехдиагональной системы методом прогонки
    bool SolveTridiagonal(const std::vector<double> &subDiagonal,
        const std::vector<double> &mainDiagonal,
        const std::vector<double> &superDiagonal,
        const std::vector<double> &rightHandSide,
        std::vector<double> &solution);
};


// Класс для отрисовки сплайнов на wxGraphicsContext
class GraphicsSplineRenderer {
public:
    // Отрисовка сплайна как пути (Path) - самый эффективный способ
    static void DrawSplinePath(wxGraphicsContext *gc,
        const std::vector<wxPoint> &points,
        double step = 1.0,
        int boundary = 0)
    {
        if (!gc || points.size() < 2) return;

        CubicSpline spline;
        if (!spline.Initialize(points, boundary)) return;

        auto splinePoints = spline.GetPoints(step);
        if (splinePoints.empty()) return;

        // Создаем графический путь
        wxGraphicsPath path = gc->CreatePath();

        // Перемещаемся в первую точку
        path.MoveToPoint(splinePoints[0].x, splinePoints[0].y);

        // Добавляем линии ко всем остальным точкам
        for (size_t i = 1; i < splinePoints.size(); i++)
        {
            path.AddLineToPoint(splinePoints[i].x, splinePoints[i].y);
        }

        // Рисуем путь
        gc->StrokePath(path);
    }

    // Отрисовка сплайна с помощью сегментов (альтернативный метод)
    static void DrawSplineSegments(wxGraphicsContext *gc,
        const std::vector<wxPoint> &points,
        double step = 1.0,
        int boundary = 0)
    {
        if (!gc || points.size() < 2) return;

        CubicSpline spline;
        if (!spline.Initialize(points, boundary)) return;

        auto splinePoints = spline.GetPoints(step);
        if (splinePoints.empty()) return;

        // Рисуем каждый сегмент отдельно
        for (size_t i = 0; i < splinePoints.size() - 1; i++)
        {
            gc->StrokeLine(splinePoints[i].x, splinePoints[i].y,
                splinePoints[i + 1].x, splinePoints[i + 1].y);
        }
    }

    // Отрисовка сплайна с заливкой области под кривой
    static void DrawFilledSplinePath(wxGraphicsContext *gc,
        const std::vector<wxPoint> &points,
        int baselineY = 0,
        double step = 1.0,
        int boundary = 0)
    {
        if (!gc || points.size() < 2) return;

        CubicSpline spline;
        if (!spline.Initialize(points, boundary)) return;

        auto splinePoints = spline.GetPoints(step);
        if (splinePoints.empty()) return;

        // Определяем базовую линию
        if (baselineY == 0 && !points.empty())
        {
            baselineY = points.back().y;
        }

        // Создаем замкнутый путь для заливки
        wxGraphicsPath path = gc->CreatePath();

        const int x = splinePoints.front().x;

        // Начинаем с левой нижней точки (на базовой линии)
        path.MoveToPoint(x, baselineY);

        // Идем вверх к началу сплайна
        path.AddLineToPoint(x, splinePoints.front().y);

        // Идем по сплайну
        for (size_t i = 1; i < splinePoints.size(); i++)
        {
            path.AddLineToPoint(splinePoints[i].x, splinePoints[i].y);
        }

        // Спускаемся к базовой линии справа
        path.AddLineToPoint(splinePoints.back().x, baselineY);

        // Закрываем путь (возвращаемся к начальной точке)
        path.CloseSubpath();

        // Заливаем и обводим
        gc->FillPath(path);  // Использует текущую кисть для заливки
        gc->StrokePath(path); // Использует текущее перо для обводки
    }

    // Отрисовка сплайна с маркерами в исходных точках
    static void DrawSplineWithMarkers(wxGraphicsContext *gc,
        const std::vector<wxPoint> &points,
        double step = 1.0,
        int boundary = 0)
    {
        if (!gc) return;

        // Рисуем сплайн
        DrawSplinePath(gc, points, step, boundary);

        // Сохраняем текущие настройки
        gc->PushState();

        // Рисуем маркеры
        gc->SetBrush(wxBrush(*wxRED));
        gc->SetPen(wxPen(*wxRED));

        for (const auto &p : points)
        {
            // Рисуем круг как путь
            wxGraphicsPath circlePath = gc->CreatePath();
            circlePath.AddCircle(p.x, p.y, 4);
            gc->FillPath(circlePath);
            gc->StrokePath(circlePath);
        }

        // Восстанавливаем настройки
        gc->PopState();
    }

    // Отрисовка сглаженного сплайна с использованием кривых Безье
    static void DrawSplineBezier(wxGraphicsContext *gc,
        const std::vector<wxPoint> &points,
        int boundary = 0)
    {
        if (!gc || points.size() < 2) return;

        // Для отрисовки через кривые Безье нужно вычислить контрольные точки
        // Это более сложная задача, но дает более гладкий результат

        CubicSpline spline;
        if (!spline.Initialize(points, boundary)) return;

        // Получаем производные в точках (для построения кривых Безье)
        std::vector<wxPoint> control1, control2;
        CalculateBezierControlPoints(points, control1, control2);

        wxGraphicsPath path = gc->CreatePath();
        path.MoveToPoint(points[0].x, points[0].y);

        for (size_t i = 0; i < points.size() - 1; i++)
        {
            // Добавляем кубическую кривую Безье
            path.AddCurveToPoint(control1[i].x, control1[i].y,
                control2[i].x, control2[i].y,
                points[i + 1].x, points[i + 1].y);
        }

        gc->StrokePath(path);
    }

private:
    // Вспомогательная функция для вычисления контрольных точек Безье
    static void CalculateBezierControlPoints(const std::vector<wxPoint> &points,
        std::vector<wxPoint> &ctrl1,
        std::vector<wxPoint> &ctrl2)
    {
        size_t n = points.size();
        ctrl1.resize(n - 1);
        ctrl2.resize(n - 1);

        // Простой метод вычисления контрольных точек
        // (можно улучшить для более точной аппроксимации)
        for (size_t i = 0; i < n - 1; i++)
        {
            double x1 = points[i].x;
            double y1 = points[i].y;
            double x2 = points[i + 1].x;
            double y2 = points[i + 1].y;

            double dist = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) / 3.0;

            // Направление от первой точки ко второй
            double angle = std::atan2(y2 - y1, x2 - x1);

            // Контрольные точки на расстоянии dist от концов
            if (i > 0)
            {
                // Учитываем предыдущий сегмент для гладкости
                double prevAngle = std::atan2(y1 - points[i - 1].y, x1 - points[i - 1].x);
                angle = (angle + prevAngle) / 2.0;
            }

            ctrl1[i].x = (int)(x1 + dist * std::cos(angle));
            ctrl1[i].y = (int)(y1 + dist * std::sin(angle));

            ctrl2[i].x = (int)(x2 - dist * std::cos(angle));
            ctrl2[i].y = (int)(y2 - dist * std::sin(angle));
        }
    }
};
