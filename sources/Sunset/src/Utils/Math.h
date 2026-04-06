// 2025/7/25 22:29:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Math
{
    template<class T>
    bool InRange(T x, T min, T max)
    {
        return x >= min && x <= max;
    }

    // Возвращает пересечение кривой с вертикальной линией по координате x
    std::pair<wxPoint, bool> GetIntersectionX(std::vector<wxPoint> &, int x);

    // Возвращает пересечение кривой с горизонтальной линией по координате y
    std::pair<wxPoint, bool> GetIntersectionY(std::vector<wxPoint> &, int y);

    int Rand(int min, int max);
}
