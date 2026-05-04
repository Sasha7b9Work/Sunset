// 2025/7/27 22:17:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Tests/SettingsTests.h"


// Вид источника/измерения
struct TypeValue
{
    enum E
    {
        Meas_C,
        Source_C,
        Meas_B,
        Source_B,
        Limit_B,
        Meas_S,
        Source_S,
        Limit_S,
        Count
    };

    E value;

    TypeValue(E v) : value(v) { }

    void operator++()
    {
        value = (E)(value + 1);
    }

};


// По какому закону изменяется диапазон : "1-2-5" или "1-2-4"
struct RowRange
{
    enum E
    {
        _124,
        _125,
        Count
    };

    RowRange(Chan::E);

    RowRange(TypeValue::E);

    operator E()const
    {
        return value;
    }

private:

    E value;
};


struct RangeI
{
    enum E
    {
        _400_500pA,
        _1nA,
        _2nA,
        _4_5nA,
        _10nA,
        _20nA,
        _40_50nA,
        _100nA,
        _200nA,
        _400_500nA,
        _1uA,
        _2uA,
        _4_5uA,
        _10uA,
        _20uA,
        _40_50uA,
        _100uA,
        _200uA,
        _400_500uA,
        _1mA,
        _2mA,
        _4_5mA,
        _10mA,
        _20mA,
        _40_50mA,
        _100mA,
        _200mA,
        _400_500mA,
        _1A,
        _2A,
        _4_5A,
        _10A,
        _20A,
        _40_50A,
        _100A,
        _200A,
        Count
    };

    E value;

    RangeI(E v) : value(v) { }

    // Если true == false, то между значением и единицами измерения нету пробела
    pchar Name(RowRange::E, bool space = true) const;

    wxString NameStep(RowRange::E) const;

    // Максимально возможное значение на данном диапазоне
    double MaxValueAbs(RowRange::E) const;

    void operator++()
    {
        value = (E)(value + 1);
    }

    static E Min(TypeValue::E);
    static E Max(TypeValue::E);

    static void FillArrayStrings(wxArrayString &, TypeValue::E, bool step);

private:

    struct Desc
    {
        pchar name_spaces[RowRange::Count];
        pchar name[RowRange::Count];
        double max_abs[RowRange::Count];
    };

    static const Desc desc[Count];
};


struct RangeU
{
    enum E
    {
        _1nV,
        _2nV,
        _4_5nV,
        _10nV,
        _20nV,
        _40_50nV,
        _100nV,
        _200nV,
        _400_500nV,
        _1uV,
        _2uV,
        _4_5uV,
        _10uV,
        _20uV,
        _40_50uV,
        _100uV,
        _200uV,
        _400_500uV,
        _1mV,
        _2mV,
        _4_5mV,
        _10mV,
        _20mV,
        _40_50mV,
        _100mV,
        _200mV,
        _400_500mV,
        _1V,
        _2V,
        _4_5V,
        _10V,
        _20V,
        _40_50V,
        _100V,
        _200V,
        _400_500V,
        _1kV,
        _2kV,
        _4_5kV,
        _10kV,
        Count
    };

    E value;

    RangeU(E v) : value(v) { }

    // Если space, то между значением и единицами измерения пробел
    pchar Name(RowRange::E, bool space = true) const;

    wxString NameStep(RowRange::E) const;

    // Максимально возможное значение на данном диапазоне
    double MaxValueAbs(RowRange::E) const;

    void operator++()
    {
        value = (E)(value + 1);
    }

    static E Min(TypeValue::E);
    static E Max(TypeValue::E);

    static void FillArrayStrings(wxArrayString &, TypeValue::E, bool step);

private:

    struct Desc
    {
        pchar name_spaces[RowRange::Count];
        pchar name[RowRange::Count];
        double max_abs[RowRange::Count];
    };

    static const Desc desc[Count];
};
