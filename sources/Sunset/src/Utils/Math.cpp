// 2025/7/25 22:29:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Math.h"
#include <random>


std::pair<wxPoint, bool> Math::GetIntersectionX(std::vector<wxPoint> &points, int x)
{
    // Проверка на пустой вектор
    if (points.empty())
    {
        return std::make_pair(wxPoint{ 0, x }, false);
    }

    // Поиск сегмента, содержащего точку x
    for (size_t i = 0; i < points.size() - 1; i++)
    {
        const wxPoint &p1 = points[i];
        const wxPoint &p2 = points[i + 1];

        // Определяем минимальное и максимальное значение x для сегмента
        int x_min = std::min(p1.x, p2.x);
        int x_max = std::max(p1.x, p2.x);

        // Проверяем, попадает ли x между x-координатами сегмента (включительно)
        if (x >= x_min && x <= x_max)
        {
            // Особый случай: вертикальный сегмент
            if (p1.x == p2.x)
            {
                return std::make_pair(wxPoint{ x, p1.y }, true); // любая точка на сегменте, выбираем p1.y
            }

            // Линейная интерполяция для нахождения y
            double t = static_cast<double>(x - p1.x) / (p2.x - p1.x);
            int y = static_cast<int>(p1.y + t * (p2.y - p1.y) + 0.5); // +0.5 для округления

            return std::make_pair(wxPoint(x, y), true);
        }
    }

    // Если точка не найдена (x вне диапазона всех сегментов)
    // Находим ближайшую точку по x
    int min_dist = abs(points[0].x - x);
    wxPoint nearest = points[0];

    for (const auto &point : points)
    {
        int dist = abs(point.x - x);
        if (dist < min_dist)
        {
            min_dist = dist;
            nearest = point;
        }
    }

    return std::make_pair(wxPoint(x, nearest.y), false); // возвращаем точку с y ближайшей вершины
}


std::pair<wxPoint, bool> Math::GetIntersectionY(std::vector<wxPoint> &points, int y)
{
    // Проверка на пустой вектор
    if (points.empty())
    {
        return std::make_pair(wxPoint{ 0, y }, false);
    }

    // Поиск сегмента, содержащего точку x
    for (size_t i = 0; i < points.size() - 1; i++)
    {
        const wxPoint &p1 = points[i];
        const wxPoint &p2 = points[i + 1];

        // Определяем минимальное и максимальное значение x для сегмента
        int y_min = std::min(p1.y, p2.y);
        int y_max = std::max(p1.y, p2.y);

        // Проверяем, попадает ли x между x-координатами сегмента (включительно)
        if (y >= y_min && y <= y_max)
        {
            // Особый случай: вертикальный сегмент
            if (p1.y == p2.y)
            {
                return std::make_pair(wxPoint{ p1.x, y }, true); // любая точка на сегменте, выбираем p1.y
            }

            // Линейная интерполяция для нахождения y
            double t = static_cast<double>(y - p1.y) / (p2.y - p1.y);
            int x = static_cast<int>(p1.x + t * (p2.x - p1.x) + 0.5); // +0.5 для округления

            return std::make_pair(wxPoint(x, y), true);
        }
    }

    // Если пересечение не найдено, возвращаем точку с x=0 и false
    return std::make_pair(wxPoint(0, y), false);
}


int Math::Rand(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}
