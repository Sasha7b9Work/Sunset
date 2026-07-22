// 2025/11/27 16:34:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Channels.h"
#include "Hardware/HAL/HAL_PINS.h"


namespace ChanB
{
    static PinOut pin3_50_V(Port::_G, Pin::_11);   // pin57 Определяет режим высокого напряжения/режим большого тока
           PinOut pinEND1B(Port::_C, Pin::_3);     // pin29 Разрешение DAC
           PinOut pinEND2B(Port::_C, Pin::_2);     // pin28 Разрешение DAC
           PinOut pinENRGB(Port::_C, Pin::_1);     // pin27 Разрешение регистра
}


void ChanB::Init()
{
    pin3_50_V.Init();
    pin3_50_V.ToHi();

    pinEND1B.Init();
    pinEND1B.ToHi();

    pinEND2B.Init();
    pinEND2B.ToHi();

    pinENRGB.Init();
    pinENRGB.ToHi();
}
