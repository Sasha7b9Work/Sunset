// 2025/09/11 14:47:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Tests/Ranges.h"


// Настройки аппаратной части


struct CalK
{
    double offset;  // Смещение
    double k;       // Коэффициент
};


// Здесь калибровочные коэффициенты
namespace DSet
{
    const CalK &Get(TypeValue::E, RangeI::E);
    const CalK &Get(TypeValue::E, RangeU::E);

    void Set(TypeValue::E, RangeI::E, const CalK);
    void Set(TypeValue::E, RangeU::E, const CalK);
}