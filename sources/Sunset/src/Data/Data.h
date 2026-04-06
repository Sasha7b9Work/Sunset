// 2025/09/07 14:30:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/Tests/DSettings.h"


class DataConverter
{
public:

    DataConverter(TypeValue::E t, RangeI::E r) : type_set{ t }, cal{ DSet::Get(type_set, r) } { }
    DataConverter(TypeValue::E t, RangeU::E r) : type_set{ t }, cal{ DSet::Get(type_set, r) } { }

    // Конвертирет отсчёт АЦП в абсолютную величину
    double Convert(int adc) const;

    // Конвертирует абсолютную величину в отсчёт АЦП
    int InverseConvert(double abs) const;

protected:

    TypeValue::E type_set;
    double      k = 1.0;    // Коэффициент наклона. Рассчитывается в конструкторе
    const CalK &cal;        // Коэффициенты калибровки устанавливаются в конструкторе
};


// Конвертирует принятое с АЦП значение прямо в амперы
class DataConverterI : public DataConverter
{
public:

    DataConverterI(TypeValue::E, RangeI::E);

private:

    RangeI range;
};


// Конвертирует принятое с АЦП значение прямо в вольты
class DataConverterU : public DataConverter
{
public:

    DataConverterU(TypeValue::E, RangeU::E);

private:

    RangeU range;
};
