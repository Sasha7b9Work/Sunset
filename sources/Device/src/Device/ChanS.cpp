// 2025/11/27 16:45:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Channels.h"
#include "Hardware/HAL/HAL_PINS.h"


namespace ChanS
{
    PinOut pinEND1P(Port::_C, Pin::_0);      // 26 Разрешение DAC
    PinOut pinEND2P(Port::_F, Pin::_10);     // 22 Разрешение DAC
    PinOut pinENRGP(Port::_F, Pin::_9);      // 21 Разрешение регистра
}


void ChanS::Init()
{
    pinEND1P.Init();
    pinEND2P.Init();
    pinENRGP.Init();
}
