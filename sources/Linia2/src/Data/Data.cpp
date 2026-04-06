// 2025/09/07 15:59:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Data.h"


double DataConverter::Convert(int adc) const
{
    double value = k * (double)adc;         // Узнаём абсолютное значение, соответствующее значению АЦП

    return (value + cal.offset) * cal.k;    // И применяем к нему коэффициенты
}


int DataConverter::InverseConvert(double abs) const
{
    double val1 = abs - cal.offset * cal.k;

    double val2 = k * cal.k;

    return (int)(val1 / val2);
}
