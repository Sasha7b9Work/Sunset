// 2025/10/24 08:54:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL_PINS.h"
#include "Device/Chips.h"


// XP10 - Источник U 50 V

namespace Source50V
{
    static const ChipDAC::E DAC_1 = ChipDAC::SOURCE_50V_PCM;

    void Init();
}


// XP13 - Источник 3 кВ

namespace Source3kV
{
    static const ChipREG::E REG = ChipREG::SOURCE_3kV;

    void Init();
}
