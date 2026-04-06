// 2025/09/07 19:18:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Data.h"


namespace DCI
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


DataConverterI::DataConverterI(TypeValue::E t, RangeI::E r) :
    DataConverter(t, r),
    range{ r }
{
    using namespace DCI;

    k = range.MaxValueAbs(RowRange(type_set)) / (double)max_ADC[type_set].value;
}
