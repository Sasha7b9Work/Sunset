// 2026/07/23 16:23:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/GPIO/PinInfo.h"


namespace PinInfo
{
    struct Info
    {
        int num_pin;
        int num_chip;
        pchar G;
    };

    static Info pins[40] =
    {
        { 1, 0, "" },
        { 2, 0, "" },
        { 3, 0, "C0" },
        { 4, 0, "" },
        { 5, 0, "B7" },
        { 6, 0, "" },
        { 7, 1, "D6" },
        { 8, 1, "A1" }
    };
}


InputPinInfo PinInfo::GetInputPinInfo(int num_pin)
{
    InputPinInfo result;

    if (std::strlen(pins[num_pin].G) != 2)
    {
        return result;
    }

    Info &pin = pins[num_pin];

    result.hw.pin_number = (pin.G[0] - 'A') * 8 + pin.G[1] - '0';

    static pchar names[4] =
    {
        "gpiochip0",
        "gpiochip1",
        "gpiochip2",
        "gpiochip3"
    };

    result.hw.chip_name = names[pin.num_chip];
    result.hw.chip = nullptr;
    result.hw.line = nullptr;

    result.last_state = false;
    result.callback = nullptr;

    return result;
}
