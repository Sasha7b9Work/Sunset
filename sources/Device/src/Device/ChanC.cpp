// 2025/11/27 17:04:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Channels.h"
#include "Hardware/HAL/HAL_PINS.h"


namespace ChanC
{
           PinOut pinRAZV_ENDU(Port::_C, Pin::_15);     //   pin9 Разрешение измерителя U блока развёртки
           PinOut pinRAZV_ENRGF(Port::_F, Pin::_0);     //  pin10 Разрешение регистра

           PinOut pinMEAS_I_ENRGI(Port::_F, Pin::_1);   //  pin11 Выбор регистра диапазона тока
    static PinOut pinMEAS_I_SPEED(Port::_G, Pin::_11);  // pin126
}


void ChanC::Init()
{
    pinRAZV_ENDU.Init();
    pinRAZV_ENRGF.Init();

    pinMEAS_I_ENRGI.Init();
    pinMEAS_I_SPEED.Init();
}


void ChanC::EmergencyStop()
{
    ChipDAC::Get(ChipDAC::CHAN_C_RANGE).ToZero();
    ChipREG::Get(ChipREG::CHAN_C).ToZero();
}


void ChanC::EmergencyStart()
{
    ChipDAC::Get(ChipDAC::CHAN_C_RANGE).WriteValue();
    ChipREG::Get(ChipREG::CHAN_C).WriteValue();
}
