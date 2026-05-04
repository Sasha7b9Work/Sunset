// 2025/7/27 22:27:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Tests/Ranges.h"


const RangeI::Desc RangeI::desc[RangeI::Count] =
{
    { { "400 pA", "500 pA" }, { "400pA", "500pA" }, { 4e-10, 5e-10 } },
    { { "1 nA",   "1 nA" },   { "1nA",   "1nA" },   { 1e-9,  1e-9 } },
    { { "2 nA",   "2 nA" },   { "2nA",   "2nA" },   { 2e-9,  2e-9 } },
    { { "4 nA",   "5 nA" },   { "4nA",   "5nA" },   { 4e-9,  5e-9 } },
    { { "10 nA",  "10 nA" },  { "10nA",  "10nA" },  { 1e-8,  1e-8 } },
    { { "20 nA",  "20 nA" },  { "20nA",  "20nA" },  { 2e-8,  2e-8 } },
    { { "40 nA",  "50 nА" },  { "40nA",  "50nА" },  { 4e-8,  5e-8 } },
    { { "100 nA", "100 nA" }, { "100nA", "100nA" }, { 1e-7,  1e-7 } },
    { { "200 nA", "200 nA" }, { "200nA", "200nA" }, { 2e-7,  2e-7 } },
    { { "400 nA", "500 nA" }, { "400nA", "500nA" }, { 4e-7,  4e-7 } },
    { { "1 uA",   "1 uA" },   { "1uA",   "1uA" },   { 1e-6,  1e-6 } },
    { { "2 uA",   "2 uA" },   { "2uA",   "2uA" },   { 2e-6,  2e-6 } },
    { { "4 uA",   "5 uA" },   { "4uA",   "5uA" },   { 4e-6,  5e-6 } },
    { { "10 uA",  "10 uA" },  { "10uA",  "10uA" },  { 1e-5,  1e-5 } },
    { { "20 uA",  "20 uA" },  { "20uA",  "20uA" },  { 2e-5,  2e-5 } },
    { { "40 uA",  "50 uA" },  { "40uA",  "50uA" },  { 4e-5,  5e-5 } },
    { { "100 uA", "100 uA" }, { "100uA", "100uA" }, { 1e-4,  1e-4 } },
    { { "200 uA", "200 uA" }, { "200uA", "200uA" }, { 2e-4,  2e-4 } },
    { { "400 uA", "500 uA" }, { "400uA", "500uA" }, { 4e-4,  5e-4 } },
    { { "1 mA",   "1 mA" },   { "1mA",   "1mA" },   { 1e-3,  1e-3 } },
    { { "2 mA",   "2 mA" },   { "2mA",   "2mA" },   { 2e-3,  2e-3 } },
    { { "4 mA",   "5 mA" },   { "4mA",   "5mA" },   { 4e-3,  5e-3 } },
    { { "10 mA",  "10 mA" },  { "10mA",  "10mA" },  { 1e-2,  1e-2 } },
    { { "20 mA",  "20 mA" },  { "20mA",  "20mA" },  { 2e-2,  2e-2 } },
    { { "40 mA",  "50 mA" },  { "40mA",  "50mA" },  { 4e-2,  5e-2 } },
    { { "100 mA", "100 mA" }, { "100mA", "100mA" }, { 1e-1,  1e-1 } },
    { { "200 mA", "200 mA" }, { "200mA", "200mA" }, { 2e-1,  2e-1 } },
    { { "400 mA", "500 mA" }, { "400mA", "500mA" }, { 4e-1,  5e-1 } },
    { { "1 A",    "1 A" },    { "1A",    "1A" },    { 1e0,   1e0  } },
    { { "2 A",    "2 A" },    { "2A",    "2A" },    { 2e0,   2e0  } },
    { { "4 A",    "5 A" },    { "4A",    "5A" },    { 4e0,   5e0  } },
    { { "10 A",   "10 A" },   { "10A",   "10A" },   { 1e1,   1e0  } },
    { { "20 A",   "20 A" },   { "20A",   "20A" },   { 2e1,   2e1  } },
    { { "40 A",   "50 A" },   { "40A",   "50A" },   { 4e1,   5e1  } },
    { { "100 A",  "100 A" },  { "100A",  "100A" },  { 1e2,   1e2  } },
    { { "200 A",  "200 A" },  { "200A",  "200A" },  { 2e2,   2e2  } }
};


pchar RangeI::Name(RowRange::E row, bool space) const
{
    if (space)
    {
        return desc[value].name_spaces[row];
    }

    return desc[value].name[row];
}


double RangeI::MaxValueAbs(RowRange::E row) const
{
    return desc[value].max_abs[row];
}


const RangeU::Desc RangeU::desc[RangeU::Count] =
{
    { { "1 nV",   "1 nV" },   { "1nV",   "1nV" },   { 1e-9, 1e-9 } },
    { { "2 nV",   "2 nV" },   { "2nV",   "2nV" },   { 2e-9, 2e-9 } },
    { { "4 nV",   "5 nV" },   { "4nV",   "5nV" },   { 4e-9, 5e-9 } },
    { { "10 nV",  "10 nV" },  { "10nV",  "10nV" },  { 1e-8, 1e-8 } },
    { { "20 nV",  "20 nV" },  { "20nV",  "20nV" },  { 2e-8, 2e-8 } },
    { { "40 nV",  "50 nV" },  { "40nV",  "50nV" },  { 4e-8, 5e-8 } },
    { { "100 nV", "100 nV" }, { "100nV", "100nV" }, { 1e-7, 1e-7 } },
    { { "200 nV", "200 nV" }, { "200nV", "200nV" }, { 2e-7, 2e-7 } },
    { { "400 nV", "500 nV" }, { "400nV", "500nV" }, { 4e-7, 5e-7 } },
    { { "1 uV",   "1 uV" },   { "1uV",   "1uV" },   { 1e-6, 1e-6 } },
    { { "2 uV",   "2 uV" },   { "2uV",   "2uV" },   { 2e-6, 2e-6 } },
    { { "4 uV",   "5 uV" },   { "4uV",   "5uV" },   { 4e-6, 5e-6 } },
    { { "10 uV",  "10 uV" },  { "10uV",  "10uV" },  { 1e-5, 1e-5 } },
    { { "20 uV",  "20 uV" },  { "20uV",  "20uV" },  { 2e-5, 2e-5 } },
    { { "40 uV",  "50 uV" },  { "40uV",  "50uV" },  { 4e-5, 5e-5 } },
    { { "100 uV", "100 uV" }, { "100uV", "100uV" }, { 1e-4, 1e-4 } },
    { { "200 uV", "200 uV" }, { "200uV", "200uV" }, { 2e-4, 2e-4 } },
    { { "400 uV", "500 uV" }, { "400uV", "500uV" }, { 4e-4, 5e-4 } },
    { { "1 mV",   "1 mV" },   { "1mV",   "1mV" },   { 1e-3, 1e-3 } },
    { { "2 mV",   "2 mV" },   { "2mV",   "2mV" },   { 2e-3, 2e-3 } },
    { { "4 mV",   "5 mV" },   { "4mV",   "5mV" },   { 4e-3, 5e-3 } },
    { { "10 mV",  "10 mV" },  { "10mV",  "10mV" },  { 1e-2, 1e-2 } },
    { { "20 mV",  "20 mV" },  { "20mV",  "20mV" },  { 2e-2, 2e-2 } },
    { { "40 mV",  "50 mV" },  { "40mV",  "50mV" },  { 4e-2, 5e-2 } },
    { { "100 mV", "100 mV" }, { "100mV", "100mV" }, { 1e-1, 1e-1 } },
    { { "200 mV", "200 mV" }, { "200mV", "200mV" }, { 2e-1, 2e-1 } },
    { { "400 mV", "500 mV" }, { "400mV", "500mV" }, { 4e-1, 5e-1 } },
    { { "1 V",    "1 V" },    { "1V",    "1V" },    { 1e0,  1e0  } },
    { { "2 V",    "2 V" },    { "2V",    "2V" },    { 2e0,  2e0  } },
    { { "4 V",    "5 V" },    { "4V",    "5V" },    { 4e0,  5e0  } },
    { { "10 V",   "10 V" },   { "10V",   "10V" },   { 1e1,  1e1  } },
    { { "20 V",   "20 V" },   { "20V",   "20V" },   { 2e1,  2e1  } },
    { { "40 V",   "50 V" },   { "40V",   "50V" },   { 4e1,  5e1  } },
    { { "100 V",  "100 V" },  { "100V",  "100V" },  { 1e2,  1e2  } },
    { { "200 V",  "200 V" },  { "200V",  "200V" },  { 2e2,  2e2  } },
    { { "400 V",  "500 V" },  { "400V",  "500V" },  { 4e2,  5e2  } },
    { { "1 kV",   "1 kV" },   { "1kV",   "1kV" },   { 1e3,  1e3  } },
    { { "2 kV",   "2 kV" },   { "2kV",   "2kV" },   { 2e3,  2e3  } },
    { { "4 kV",   "5 kV" },   { "4kV",   "5kV" },   { 4e3,  5e3  } },
    { { "10 kV",  "10 kV" },  { "10kV",  "10kV" },  { 1e4,  1e4  } }
};


pchar RangeU::Name(RowRange::E row, bool space) const
{
    if (space)
    {
        return desc[value].name_spaces[row];
    }

    return desc[value].name[row];
}


double RangeU::MaxValueAbs(RowRange::E row) const
{
    return desc[value].max_abs[row];
}


wxString RangeI::NameStep(RowRange::E row) const
{
    wxString name = Name(row);

    wxString val = name.BeforeFirst(' ');

    wxString units = name.AfterFirst(' ');

    int int_value = 0;

    val.ToInt(&int_value);

    if (int_value == 50)
    {
        val = "2.5";
    }
    else if (int_value >= 20)
    {
        val = wxString::Format("%d", int_value / 20);
    }
    else
    {
        val = wxString::Format("%d", int_value * 100 / 2);

        if (units[0] == 'A')        units = 'm' + units;
        else if (units[0] == 'm')   units = "uA";
        else if (units[0] == 'u')   units = "nA";
        else if (units[0] == 'n')   units = "pA";
    }

    return val + " " + units;
}


wxString RangeU::NameStep(RowRange::E row) const
{
    wxString name = Name(row);

    wxString val = name.BeforeFirst(' ');

    wxString units = name.AfterFirst(' ');

    int int_value = 0;

    val.ToInt(&int_value);

    if (int_value == 50)
    {
        val = "2.5";
    }
    else if (int_value >= 20)
    {
        val = wxString::Format("%d", int_value / 20);
    }
    else
    {
        val = wxString::Format("%d", int_value * 100 / 2);

        if (units[0] == 'V')        units = 'm' + units;
        else if (units[0] == 'k')   units = "V";
        else if (units[0] == 'm')   units = "uV";
        else if (units[0] == 'u')   units = "nV";
        else if (units[0] == 'n')   units = "pV";
    }

    return val + " " + units;
}


RangeI::E RangeI::Min(TypeValue::E type)
{
    struct StructMin
    {
        StructMin(RangeI::E r) : range{r} { }
        RangeI::E range;
    };

    static const StructMin min[TypeValue::Count] =
    {
        _10nA,
        _10nA,
        _40_50nA,
        _40_50nA,
        _40_50nA,
        _40_50nA,
        _40_50nA,
        _40_50nA
    };

    return min[type].range;
}


RangeI::E RangeI::Max(TypeValue::E type)
{
    struct StructMax
    {
        StructMax(RangeI::E r) : range{r} { }
        RangeI::E range;
    };

    static const StructMax max[TypeValue::Count] =
    {
        _40_50A,
        _40_50A,
        _10A,
        _10A,
        _10A,
        _10mA,
        _10mA,
        _10mA
    };

    return max[type].range;
}


RangeU::E RangeU::Min(TypeValue::E type)
{
    struct StructMin
    {
        StructMin(RangeU::E r) : range{r} { }
        RangeU::E range;
    };

    static const StructMin min[TypeValue::Count] =
    {
        _400_500mV,
        _400_500mV,
        _1V,
        _1V,
        _1V,
        _1V,
        _1V,
        _1V
    };

    return min[type].range;
}


RangeU::E RangeU::Max(TypeValue::E type)
{
    struct StructMax
    {
        StructMax(RangeU::E r) : range{r} { }
        RangeU::E range;
    };

    static const StructMax max[TypeValue::Count] =
    {
        _2kV,
        _2kV,
        _40_50V,
        _40_50V,
        _40_50V,
        _40_50V,
        _40_50V,
        _40_50V
    };

    return max[type].range;
}


void RangeI::FillArrayStrings(wxArrayString &arr, TypeValue::E type, bool steps)
{
    arr.clear();
    for (int i = RangeI::Min(type); i <= RangeI::Max(type); i++)
    {
        if (steps)
        {
            arr.push_back(RangeI((RangeI::E)i).NameStep(RowRange(type)));
        }
        else
        {
            arr.push_back(RangeI((RangeI::E)i).Name(RowRange(type)));
        }
    }
}


void RangeU::FillArrayStrings(wxArrayString &arr, TypeValue::E type, bool steps)
{
    arr.clear();

    for (int i = RangeU::Min(type); i <= RangeU::Max(type); i++)
    {
        if (steps)
        {
            arr.push_back(RangeU((RangeU::E)i).NameStep(RowRange(type)));
        }
        else
        {
            arr.push_back(RangeU((RangeU::E)i).Name(RowRange(type)));
        }
    }
}


RowRange::RowRange(TypeValue::E type)
{
    struct StructRow
    {
        StructRow(E r) : row{r} { }
        E row;
    };

    static const StructRow row[TypeValue::Count] =
    {
        _125,
        _125,
        _124,
        _124,
        _124,
        _124,
        _124,
        _124
    };

    value = row[type].row;
}


RowRange::RowRange(Chan::E ch)
{
    static const E r[Chan::Count] =
    {
        _125,
        _124,
        _124,
        _124
    };

    value = r[ch];
}
