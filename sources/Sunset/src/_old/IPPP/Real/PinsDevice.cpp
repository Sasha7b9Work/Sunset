// 2025/10/23 15:09:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Real/PinsDevice.h"


PinInDevice pin3000V(PinDevice::_3000V);
PinInDevice pin50_P(PinDevice::_50_P);
PinInDevice pin50_N(PinDevice::_50_N);
PinInDevice pin200_P(PinDevice::_200_P);
PinInDevice pin200_N(PinDevice::_200_N);
PinInDevice pin20_P(PinDevice::_20_P);
PinInDevice pin20_N(PinDevice::_20_N);

PinInDevice *pinsD[] =
{
    &pin3000V,
    &pin50_P,
    &pin50_N,
    &pin200_P,
    &pin200_N,
    &pin20_P,
    &pin20_N,
    nullptr
};


pchar PinDevice::Name() const
{
    static const pchar names[] =
    {
        "3000V",
        "50P",
        "50N",
        "200P",
        "200N",
        "20P",
        "20N"
    };

    if (value >= 0 &&value < Count)
    {
        return names[value];
    }

    LOG_ERROR("Bad index");

    return "Bad name";
}


void PinDevice::SetState(bool)
{

}
