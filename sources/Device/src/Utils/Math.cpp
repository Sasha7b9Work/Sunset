// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Math.h"


uint Math::Pow10(int pow)
{
    uint result = 1;

    while(pow--)
    {
        result *= 10;
    }

    return result;
}
