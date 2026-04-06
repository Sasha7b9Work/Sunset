// 2025/09/04 16:37:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Tests/SettingsTests.h"


const Chan ChC(Chan::_C);
const Chan ChB(Chan::_B);
const Chan ChS(Chan::_S);


bool Category::IsBCE()
{
    E v = Current();

    return (v == BCE_N || v == BCE_P || v == BCSE_N || v == BCSE_P);
}


bool Category::IsGDS()
{
    E v = Current();

    return (v == GDS_N || v == GDS_P || v == GDBS_N || v == GDBS_P);
}


pchar Chan::Name() const
{
    static const pchar names[Count] =
    {
        "C",
        "B",
        "S",
        "E"
    };

    return names[value];
}


pchar ModeSource::Name(E mode)
{
    return mode == U ? "U" : "I";
}


pchar ModeMeas::Name(E mode)
{
    return mode == U ? "U" : "I";
}


pchar TypeScan::NameShort(E v)
{
    static const pchar names[Count] =
    {
        "IMP_P",
        "IMP_N",
        "DC_P",
        "DC_N",
        "SYN_P",
        "SYN_N",
        "AC"
    };

    return names[v];
}


pchar TypeScan::NameFileICO(E v)
{
    static pchar names[Count] =
    {
        "icons/graphs/imp_up.ico",
        "icons/graphs/imp_down.ico",
        "icons/graphs/triang_up.ico",
        "icons/graphs/triang_down.ico",
        "icons/graphs/sin_hi.ico",
        "icons/graphs/sin_lo.ico",
        "icons/graphs/sin.ico"
//        "icons/graphs/imp_hi_osc.ico",
//        "icons/graphs/imp_hi_IVC.ico"
    };

    if (v < Count)
    {
        return names[v];
    }

    LOG_ERROR("Very big index");

    return "";
}


pchar TypeScan::_NameGUI(E v)
{
    static pchar names[Count] =
    {
        "Положительные импульсы",
        "Отрицательные импульсы",
        "Положительное постоянное",
        "Отрицательное постоянное",
        "Положительные полуволны",
        "Отрицательные полуволны",
        "Синусоидальное"
    };

    if (v < Count)
    {
        return names[v];
    }

    LOG_ERROR("Very big index");

    return "";
}


const pchar StateJack::names[StateJack::Count][2] =
{
    { "C",        "C" },
    { "B",        "B" },
    { "S",        "S" },
    { "E",        "E" },
    { "общий",    "GND" },
    { "общий 1к", "GND_1K"},
    { "обрыв",    "BREAK" }
};


pchar StateJack::NameHardware(E v)
{
    return names[v][1];
}


pchar StateJack::NameGUI(E v)
{
    return names[v][0];
}


void StateJack::PrepareArray(wxArrayString &arr, E v1, E v2, E v3, E v4)
{
    arr.Clear();
    arr.push_back(NameGUI(v1));
    arr.push_back(NameGUI(v2));
    if (v3 != Count)
    {
        arr.push_back(NameGUI(v3));
    }
    if (v4 != Count)
    {
        arr.push_back(NameGUI(v4));
    }
}


Category::E Category::Current()
{
    return Category::Count;
}


bool Chan::IsVisible() const
{
    Category::E cat = Category::Current();

    if (value == Chan::_B)
    {
        if (cat == Category::Diod || cat == Category::Resistor || cat == Category::Capacitor)
        {
            return false;
        }
    }

    if (value == Chan::_S)
    {
        if (cat == Category::Diod ||
            cat == Category::Resistor ||
            cat == Category::Capacitor ||
            cat == Category::BCE_N ||
            cat == Category::BCE_P ||
            cat == Category::GDS_N ||
            cat == Category::GDS_P)
        {
            return false;
        }
    }

    return true;
}


bool TypeCommutation::IsInternal()
{
    return false;
}


pchar MicroChip::Name(E v)
{
    static const pchar names[Count] =
    {
        "DAC_5300_1",
        "DAC_5300_2",
        "DAC_5443",
        "DAC_5531"
    };

    return names[v];
}
