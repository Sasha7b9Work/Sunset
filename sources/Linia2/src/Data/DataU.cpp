// 2025/09/07 19:17:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Data.h"


namespace DCU
{
    struct StructADC
    {
        StructADC(int v) : value{ v } { }
        int value;
    };

    // Это значение для данного измерения либо источника приходит с АЦП при максимально возможном значении на входе.
    // Ему соответствует минимальное значение -max_value_ADC
    static const StructADC max_ADC[TypeValue::Count] =
    {
        ((1 << 18) - 1),
        ((1 << 18) - 1),
        ((1 << 18) - 1),
        ((1 << 18) - 1),
        ((1 << 18) - 1),
        ((1 << 18) - 1),
        ((1 << 18) - 1),
        ((1 << 18) - 1),
    };
}


DataConverterU::DataConverterU(TypeValue::E t, RangeU::E r) :
    DataConverter(t, r),
    range{ r }
{
    using namespace DCU;

    k = range.MaxValueAbs(RowRange(type_set)) / (double)max_ADC[type_set].value;
}
