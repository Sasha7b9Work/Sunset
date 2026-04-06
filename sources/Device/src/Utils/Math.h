// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Bit
{
    template<typename T>
    bool Get(T value, int bit)
    {
        return ((value >> bit) & 0x01) != 0;
    }

    template<typename T>
    void Set(T &value, int bit)
    {
        value |= 1 << bit;
    }

    template<typename T>
    void Clear(T &value, int bit)
    {
        value &= ~(1 << bit);
    }
};


namespace Math
{
    // Вычисление 10**pow.
    uint Pow10(int pow);

    template<class T> T Limitation(T *value, T min, T max)
    {
        if(*value < min)
        {
            *value = min;
        }
        else if(*value > max)
        {
            *value = max;
        }

        return *value;
    }

    template<class T> int Sign(T x) { if(x > (T)(0)) { return 1; } if(x < (T)(0)) { return -1; } return 0; }
};
