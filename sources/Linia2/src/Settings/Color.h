// 2023/08/10 13:15:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Painter.h"
#include "Controls/Buttons.h"


class ButtonColor;


struct Color
{
    Color(uint8 r, uint8 g, uint8 b, uint8 brightness = 255)
    {
        value = b + (uint)(g << 8) + (uint)(r << 16) + (uint)(brightness << 24);
    }

    Color(uint _value = 0) : value(_value) { }

    int GetRed() const
    {
        return (int)(uint8)(value >> 16);
    }

    int GetGreen() const
    {
        return (int)(uint8)(value >> 8);
    }

    int GetBlue() const
    {
        return (int)(uint8)(value);
    }

    int GetBrightness() const
    {
        return (int)(uint8)(value >> 24);
    }

    float GetBrightnessF() const
    {
        return (float)GetBrightness() / 255.0f;
    }

    static Color FromUint(uint _value)
    {
        return Color(_value);
    }

    uint value;

    bool operator==(const Color &rhs) const
    {
        return value == rhs.value;
    }
};


struct TypeColor
{
    enum E
    {
        Red,
        Green
    };
};
