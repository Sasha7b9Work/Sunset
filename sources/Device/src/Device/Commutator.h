// 2025/10/24 08:56:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL_PINS.h"
#include "Device/Chips.h"


// XP12 - Коммутатор

namespace Commutator
{
    void Init();

    void PeriodicTask();

    extern ChipREG &REG;
}
