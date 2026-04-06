// 2026/3/15 21:25:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


template<class T>
struct FinderMinMax
{
    FinderMinMax(T _min_, T _max_) : min(_max_), max(_min_) {}
    void Push(T value)
    {
        counter++;
        if (value < min)
        {
            min = value;
        }
        if (value > max)
        {
            max = value;
        }
    }
    T Min() const
    {
        return min;
    }
    T Max() const
    {
        return max;
    }
    int Size() const
    {
        return counter;
    }
private:
    T min;
    T max;
    int counter = 0;
};
