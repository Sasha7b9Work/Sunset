// 2025/10/24 09:30:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL_PINS.h"
#include "Device/Chips.h"


// Интерфейсы трёх блоков каналов

// Канал базы
namespace ChanB
{
    static const ChipDAC::E DAC_1 = ChipDAC::CHAN_B_1;
    static const ChipDAC::E DAC_2 = ChipDAC::CHAN_B_2;

    static const ChipREG::E REG = ChipREG::CHAN_B;

    void Init();
}


// XP5 - Канал подложки
namespace ChanS
{
    static const ChipDAC::E DAC_1 = ChipDAC::CHAN_S_1;
    static const ChipDAC::E DAC_2 = ChipDAC::CHAN_S_2;

    static const ChipREG::E REG = ChipREG::CHAN_S;

    void Init();
}


// XP6, XP7 - Измеритель тока, формирователь развёртки (канал коллектора)
namespace ChanC
{
    static const ChipDAC::E DAC_RANGE = ChipDAC::CHAN_C_RANGE;

    static const ChipREG::E REG_U = ChipREG::CHAN_C;
    static const ChipREG::E REG_I = ChipREG::MEAS_I;

    void Init();

    // Привести состояние всех элементов в безопасное состояние - записать нули
    void EmergencyStop();
    void EmergencyStart();
}
