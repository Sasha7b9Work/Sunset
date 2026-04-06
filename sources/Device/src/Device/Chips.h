// 2025/10/15 16:05:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL_PINS.h"


/*
*   Здесь всё, что относится к управлению регистрами и ЦАП-ми всех блоков
*/


struct Chip
{
    Chip(uint _l, PinOut *_cs, PinOut *_clk, PinOut *_dat, bool _level_cs) :
        cs(_cs), clk(_clk), dat(_dat), level_cs(_level_cs), length(_l)
    {
        Init();
    }

    void Init();

    void SetLength(uint _length)
    {
        length = _length;
    }

    // Запись значения в буферный регистр. Перезапись в регистр устойства происходит по
    // команде Start()
    void WriteValue(uint);

    // По этой команде происходит запись нулевого значения в регистр на плате
    void ToZero();

    // По этой команде происходит запись значения из буферного регистра в аппаратный регист на плате
    void WriteValue();

protected:

    PinOut *cs;
    PinOut *clk;
    PinOut *dat;
    bool   level_cs;                // Если false, то активный уровень выбора - 0, иначе - 1
    uint length = 0;
    uint buffer_value = 0;          // Здесь хранится буферное значение, которое по команде Start() переписываетя в аппаратный регистр на плате

    // Непросредственная запись значения в железный регистр на плате
    void WriteValueRAW(uint);
};


struct ChipDAC : public Chip
{
    enum E
    {
        CHAN_C_PCM,     // Динамический ЦАП - в него могут засылаться значения во время измерения
        CHAN_C_RANGE,
        CHAN_B_1,
        CHAN_B_2,
        CHAN_S_1,
        CHAN_S_2,
        SOURCE_50V_PCM, // Динамический ЦАП - в него могут засылаться значения во время проведения измерения
        Count
    };

    ChipDAC(E v, uint _l, PinOut *_cs, PinOut *_clk, PinOut *_dat, bool _level_cs) : Chip(_l, _cs, _clk, _dat, _level_cs), type(v)  { }

    static ChipDAC &Get(E);

private:

    E type;

    static ChipDAC dacs[10];
};


struct ChipREG : public Chip
{
    enum E
    {
        SOURCE_3kV,
        COMMUTATOR,
        CHAN_C,
        CHAN_B,
        CHAN_S,
        MEAS_I,
        Count
    };

    ChipREG(E v, uint _l, PinOut *_cs, PinOut *_clk, PinOut *_dat, bool _level_cs) :
        Chip(_l, _cs, _clk, _dat, _level_cs), type(v) { }

    static ChipREG &Get(E);

private:

    E type;

    static ChipREG regs[10];
};
