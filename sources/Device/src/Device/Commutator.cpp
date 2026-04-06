// 2025/10/24 08:56:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Commutator.h"
#include "Device/PCM/SCPI.h"


namespace Commutator
{
    ChipREG &REG = ChipREG::Get(ChipREG::COMMUTATOR);

    static PinIn pin3000VE(Port::_C, Pin::_4);     // 44
    static PinIn pin200EN(Port::_C, Pin::_5);      // 45
    static PinIn pin200EP(Port::_B, Pin::_0);      // 46
    static PinIn pin20EN(Port::_B, Pin::_1);       // 47
    static PinIn pin20EP(Port::_B, Pin::_2);       // 48
                 PinOut pinENRGK(Port::_F, Pin::_7);      // 19

    static StructPinIn pins[] =
    {
        { &pin3000VE, "3000V" },
        { &pin200EN,  "200N" },
        { &pin200EP,  "200P" },
        { &pin20EP,   "20P" },
        { &pin20EN,   "20N" },
        { nullptr,    nullptr }
    };
}


void Commutator::Init()
{
    pin3000VE.Init();
    pin200EN.Init();
    pin200EP.Init();
    pin20EN.Init();
    pin20EP.Init();
    pinENRGK.Init();

    REG.WriteValue(0);
}


void Commutator::PeriodicTask()
{
    StructPinIn *pin = &pins[0];

    while (pin->pin)
    {
        bool prev_state = pin->pin->PrevState();
        bool state = pin->pin->Get();

        if (prev_state != state)
        {
            PCM::SCPI::Send("PININ:%s:STATE %d", pin->name, state ? "1" : "0");
        }
        
        pin++;
    }
}
