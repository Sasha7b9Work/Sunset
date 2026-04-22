// 2026/03/14 01:12:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainPanel/PanelMeasures/Graphics/Splines.h"


// Реализация CubicSpline
CubicSpline::CubicSpline() : valid(false)
{
}

bool CubicSpline::Initialize(const std::vector<wxPoint> &points,
    int boundary,
    double leftDerivative,
    double rightDerivative)
{
    std::vector<wxRealPoint> realPoints;
    for (const auto &p : points)
    {
        realPoints.emplace_back(p.x, p.y);
    }
    return Initialize(realPoints, boundary, leftDerivative, rightDerivative);
}

bool CubicSpline::Initialize(const std::vector<wxRealPoint> &points,
    int boundary,
    double leftDerivative,
    double rightDerivative)
{
    valid = false;

    // Проверка на минимальное количество точек
    if (points.size() < 2) return false;

    size_t n = points.size() - 1; // количество интервалов

    // Заполняем массивы x и y
    x.clear();
    y.clear();
    for (const auto &p : points)
    {
        x.push_back(p.x);
        y.push_back(p.y);
    }

    // Проверяем, что x монотонно возрастают
    for (size_t i = 1; i < x.size(); i++)
    {
        if (x[i] <= x[i - 1]) return false;
    }

    // Инициализация массивов для решения системы
    std::vector<double> h(n);      // шаги между точками

    // Вычисляем шаги
    for (size_t i = 0; i < n; i++)
    {
        h[i] = x[i + 1] - x[i];
    }

    // Подготовка системы для натурального сплайна
    if (boundary == 0)
    {
        // Натуральный сплайн (вторая производная = 0 на концах)
        std::vector<double> sub(n - 1, 0.0);    // поддиагональ
        std::vector<double> main(n - 1, 2.0);   // главная диагональ
        std::vector<double> sup(n - 1, 0.0);    // наддиагональ
        std::vector<double> rhs(n - 1, 0.0);    // правая часть

        for (size_t i = 1; i < n; i++)
        {
            sub[i - 1] = h[i - 1] / (h[i - 1] + h[i]);
            main[i - 1] = 2.0;
            sup[i - 1] = h[i] / (h[i - 1] + h[i]);
            rhs[i - 1] = 6.0 * ((y[i + 1] - y[i]) / h[i] - (y[i] - y[i - 1]) / h[i - 1]) / (h[i - 1] + h[i]);
        }

        // Решаем систему для вторых производных
        std::vector<double> solution;
        if (!SolveTridiagonal(sub, main, sup, rhs, solution))
        {
            return false;
        }

        // Заполняем массив вторых производных
        std::vector<double> d2y(n + 1, 0.0);
        for (size_t i = 0; i < solution.size(); i++)
        {
            d2y[i + 1] = solution[i];
        }
        d2y[0] = 0.0;      // натуральные граничные условия
        d2y[n] = 0.0;

        // Вычисляем коэффициенты сплайна
        a.resize(n);
        b.resize(n);
        vec_c.resize(n);
        vec_d.resize(n);

        for (size_t i = 0; i < n; i++)
        {
            a[i] = (d2y[i + 1] - d2y[i]) / (6.0 * h[i]);
            b[i] = d2y[i] / 2.0;
            vec_c[i] = (y[i + 1] - y[i]) / h[i] - (2.0 * d2y[i] + d2y[i + 1]) * h[i] / 6.0;
            vec_d[i] = y[i];
        }
    }
    else
    {
        // Закрепленный сплайн (заданы первые производные на концах)
        std::vector<double> sub(n + 1, 0.0);
        std::vector<double> main(n + 1, 2.0);
        std::vector<double> sup(n + 1, 0.0);
        std::vector<double> rhs(n + 1, 0.0);

        // Заполняем систему
        for (size_t i = 1; i < n; i++)
        {
            sub[i] = h[i - 1];
            main[i] = 2.0 * (h[i - 1] + h[i]);
            sup[i] = h[i];
            rhs[i] = 3.0 * ((y[i + 1] - y[i]) / h[i] * h[i - 1] + (y[i] - y[i - 1]) / h[i - 1] * h[i]);
        }

        // Граничные условия
        main[0] = 2.0 * h[0];
        sup[0] = h[0];
        rhs[0] = 3.0 * ((y[1] - y[0]) / h[0] - leftDerivative);

        main[n] = 2.0 * h[n - 1];
        sub[n] = h[n - 1];
        rhs[n] = 3.0 * (rightDerivative - (y[n] - y[n - 1]) / h[n - 1]);

        // Решаем систему для производных
        std::vector<double> derivatives;
        if (!SolveTridiagonal(sub, main, sup, rhs, derivatives))
        {
            return false;
        }

        // Вычисляем коэффициенты сплайна
        a.resize(n);
        b.resize(n);
        vec_c.resize(n);
        vec_d.resize(n);

        for (size_t i = 0; i < n; i++)
        {
            a[i] = (derivatives[i + 1] + derivatives[i] - 2.0 * (y[i + 1] - y[i]) / h[i]) / (h[i] * h[i]);
            b[i] = (3.0 * (y[i + 1] - y[i]) / h[i] - 2.0 * derivatives[i] - derivatives[i + 1]) / h[i];
            vec_c[i] = derivatives[i];
            vec_d[i] = y[i];
        }
    }

    valid = true;
    return true;
}

double CubicSpline::Evaluate(double t) const
{
    if (!valid) return 0.0;

    // Находим интервал, содержащий t
    size_t i = 0;
    while (i < x.size() - 1 && t > x[i + 1])
    {
        i++;
    }

    if (i >= x.size() - 1) i = x.size() - 2;

    // Вычисляем значение сплайна
    double dx = t - x[i];
    return a[i] * dx * dx * dx + b[i] * dx * dx + vec_c[i] * dx + vec_d[i];
}

std::vector<wxPoint> CubicSpline::GetPoints(double step) const
{
    std::vector<wxPoint> result;
    if (!valid || x.empty()) return result;

    // Генерируем точки от x[0] до x.back() с заданным шагом
    for (double t = x.front(); t <= x.back() + step / 2; t += step) //-V1034
    {
        double val = Evaluate(t);
        result.emplace_back(static_cast<int>(t), static_cast<int>(val));
    }

    return result;
}


bool CubicSpline::SolveTridiagonal(const std::vector<double> &sub,
    const std::vector<double> &main,
    const std::vector<double> &sup,
    const std::vector<double> &rhs,
    std::vector<double> &solution)
{
    size_t n = main.size();
    if (n == 0) return false;

    solution.resize(n);
    std::vector<double> c(n, 0.0);
    std::vector<double> d(n, 0.0);

    // Прямой ход метода прогонки
    c[0] = sup[0] / main[0];
    d[0] = rhs[0] / main[0];

    for (size_t i = 1; i < n; i++)
    {
        double denom = main[i] - sub[i] * c[i - 1];
        if (std::abs(denom) < 1e-10) return false;

        if (i < n - 1)
        {
            c[i] = sup[i] / denom;
        }
        d[i] = (rhs[i] - sub[i] * d[i - 1]) / denom;
    }

    // Обратный ход
    solution[n - 1] = d[n - 1];
    for (int64 i = static_cast<int64>(n) - 2; i >= 0; i--)
    {
        solution[(size_t)i] = d[(size_t)i] - c[(size_t)i] * solution[(size_t)(i + 1)];
    }

    return true;
}
