// 2025/08/29 18:39:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class wxArrayString;


// Категория испытуемого прибора
struct Category
{
    enum E
    {
        BCE_N,          // Биполярный NPN-транзистор
        BCE_P,          // Биполярный PNP-транзистор
        GDS_N,          // Полевой NMOS-транзистор
        GDS_P,          // Полевой PMOS-транзистор
        BCSE_N,         // Биполярный NPN-транзистор четырёхполюсный
        BCSE_P,         // Биполярный PNP-транзистор четырёхполюсный
        GDBS_N,         // Полевой NMOS-транзистор четырёхполюсный
        GDBS_P,         // Полевой PMOS-транзистор четырёхполюсный
        Diod,
        Thyristor,
        Resistor,
        Capacitor,
        Count
    };

    static E Current();

    static bool IsBCE();        // Биполярный транзистор
    static bool IsGDS();        // Полевой транзистор
};


struct Chan
{
    enum E
    {
        _C,      // Коллектор
        _B,      // База
        _S,      // Подложка
        _E,      // Эмиттер - общий
        Count
    };

    explicit Chan(E v) : value(v) { }

    E value;

    bool IsVisible() const;

    pchar Name() const;

    bool IsBS() const
    {
        return value == _B || value == _S;
    }
};


extern const Chan ChC;
extern const Chan ChB;
extern const Chan ChS;


struct StateJack
{
    enum E
    {
        _C,
        _B,
        _S,
        _E,
        General,        // Общий
        General_1k,     // Общий 1к
        Break,          // Обрыв
        Count
    };

    static void PrepareArray(wxArrayString &, E, E, E = Count, E = Count);

    // Эти обозначения используются для сообщений в аппаратной части
    static pchar NameHardware(E);

    // Эти обозначения используются в ГИП
    static pchar NameGUI(E);

private:

    static const pchar names[Count][2];
};


// Тип коммутации
struct TypeCommutation
{
    enum E
    {
        Int,            // Внутренняя
        Ext,            // Внешняя
        Count
    };

    static bool IsInternal();
};


// Режим источника
struct ModeSource
{
    enum E
    {
        U,
        I,
        Count
    };

    static pchar Name(E);
};


// Режим измерителя
struct ModeMeas
{
    enum E
    {
        U,
        I,
        Count
    };

    static pchar Name(E);
};


// Тип развёртки
struct TypeScan
{
    enum E
    {
        ImpulsePos,
        ImpulseNeg,
        DCPos,
        DCNeg,
        SYNPos,
        SYNNeg,
        AC,
        Count
    };

    static pchar NameShort(E);

    static pchar _NameGUI(E);

    static pchar NameFileICO(E);
};


struct MicroChip
{
    enum E
    {
        DAC_5300_1,
        DAC_5300_2,
        DAC_5443,
        DAC_5531,
        Count
    };

    static pchar Name(E);
};
