// 2025/10/15 16:04:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Chips.h"
#include "Device/Commutator.h"
#include "Device/Channels.h"
#include "Device/Sources.h"
#include "Device/FPGA.h"
#include "Hardware/Timer.h"


/*
+   ENRGV   // Регистр Источник 3 кВ
+   END1B   // DAC Канал B
+   END2B   //
+   ENRGB   // Регистр Канал B
+   END1P   // DAC Канала S
+   END2P   //
+   ENRGP   // Регистр Канал S
+   ENRGI   // Регистр измерителя тока
+   ENDU    // Формирователь развёртки
+   ENRGF   // Регистр формирователя
+   ENRGK   // Регистр коммутатора
*/


namespace ChanB
{
    extern PinOut pinEND1B;
    extern PinOut pinEND2B;
    extern PinOut pinENRGB;
}


namespace ChanS
{
    extern PinOut pinEND1P;
    extern PinOut pinEND2P;
    extern PinOut pinENRGP;
}


namespace ChanC
{
    extern PinOut pinRAZV_ENDU;
    extern PinOut pinRAZV_ENRGF;
    extern PinOut pinMEAS_I_ENRGI;
}


namespace Source3kV
{
    extern PinOut pinENRGV;
}


namespace Commutator
{
    extern PinOut pinENRGK;
}


ChipDAC ChipDAC::dacs[10] =
{   //                               cs                     clk        dat
    { ChipDAC::CHAN_C_PCM,     12, nullptr,              nullptr,    nullptr,    false },  // 0 Канал C. Управляется с PCM
    { ChipDAC::CHAN_C_RANGE,   12, &ChanC::pinRAZV_ENDU, &pCLK2_DAC, &pDAT2_DAC, false },  // 1 Канал C. Диапазон
    { ChipDAC::CHAN_B_1,       14, &ChanB::pinEND1B,     &pCLK1_DAC, &pDAT1_DAC, true  },  // 2 Канал B
    { ChipDAC::CHAN_B_2,        8, &ChanB::pinEND2B,     &pCLK1_DAC, &pDAT1_DAC, true  },  // 3 Канал B
    { ChipDAC::CHAN_S_1,       12, &ChanS::pinEND1P,     &pCLK1_DAC, &pDAT1_DAC, false },  // 4 Канал S
    { ChipDAC::CHAN_S_2,       12, &ChanS::pinEND2P,     &pCLK1_DAC, &pDAT1_DAC, false },  // 5 Канал S
    { ChipDAC::SOURCE_50V_PCM, 12, nullptr,              nullptr,    nullptr,    false },  // 6 Источник 50 В. Управляется с PCM
    { ChipDAC::Count,           0, nullptr,              nullptr,    nullptr,    false },
    { ChipDAC::Count,           0, nullptr,              nullptr,    nullptr,    false },
    { ChipDAC::Count,           0, nullptr,              nullptr,    nullptr,    false }
};


ChipREG ChipREG::regs[10] =
{
    { ChipREG::SOURCE_3kV, 24, &Source3kV::pinENRGV,    &pCLK2_DAC, &pDAT2_DAC, false },  // 0 Источник 3кВ
    { ChipREG::COMMUTATOR, 16, &Commutator::pinENRGK,   &pCLK2_DAC, &pDAT2_DAC, false },  // 1 Коммутатор
    { ChipREG::CHAN_C,      8, &ChanC::pinRAZV_ENRGF,   &pCLK2_DAC, &pDAT2_DAC, false },  // 2 Формирователь развёртки
    { ChipREG::CHAN_B,     32, &ChanB::pinENRGB,        &pCLK1_DAC, &pDAT1_DAC, false },  // 3 Канал B
    { ChipREG::CHAN_S,     32, &ChanS::pinENRGP,        &pCLK1_DAC, &pDAT1_DAC, false },  // 4 Канал S
    { ChipREG::MEAS_I,     16, &ChanC::pinMEAS_I_ENRGI, &pCLK2_DAC, &pDAT2_DAC, false },  // 5 Измеритель тока
    { ChipREG::Count,       0, nullptr,                 nullptr,    nullptr,    false },
    { ChipREG::Count,       0, nullptr,                 nullptr,    nullptr,    false },
    { ChipREG::Count,       0, nullptr,                 nullptr,    nullptr,    false },
    { ChipREG::Count,       0, nullptr,                 nullptr,    nullptr,    false }
};


void Chip::Init()
{
    if (clk)
    {
        cs->Set(!level_cs);
        clk->ToHi();
    }
}


void Chip::WriteValue(uint value)
{
    buffer_value = value;

    if (FPGA::IsScanning())                 // Если развёртка запущена - сразу записываем значение в аппаратный регистр
    {
        WriteValueRAW(buffer_value);
    }
}


void Chip::WriteValueRAW(uint value)
{
    if (!clk)
    {
//        LOG_ERROR("Bad record");
        return;
    }

#define TIME_WAIT 2
#define WAIT      meter.WaitForUSandReset(TIME_WAIT)
#define WAIT_HALF meter.WaitForUSandReset(TIME_WAIT / 2)

    TimeMeterUS meter;

    clk->ToLow();

    WAIT;

    cs->Set(level_cs);
    
    WAIT_HALF;

    for (int bit = (int)length - 1; bit >= 0; bit--)
    {
        WAIT_HALF;

        clk->ToLow();

        WAIT_HALF;

        dat->Set(_GET_BIT(value, bit) != 0);

        WAIT_HALF;

        clk->ToHi();

        WAIT_HALF;
    }
    
    WAIT_HALF;

    clk->ToLow();

    WAIT;

    cs->Set(!level_cs);
}


void Chip::ToZero()
{
    WriteValueRAW(0);
}


void Chip::WriteValue()
{
    WriteValueRAW(buffer_value);
}


ChipDAC &ChipDAC::Get(E type)
{
    return dacs[type];
}


ChipREG &ChipREG::Get(E type)
{
    return regs[type];
}
