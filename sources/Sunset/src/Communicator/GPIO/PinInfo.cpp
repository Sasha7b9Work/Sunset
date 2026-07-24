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

    static const Info pins[40] =
    {
        {  1, 0, ""   },
        {  2, 0, ""   },
        {  3, 0, "C0" },
        {  4, 0, ""   },
        {  5, 0, "B7" },
        {  6, 0, ""   },
        {  7, 1, "D6" },
        {  8, 1, "A1" },
        {  9, 0, ""   },
        { 10, 1, "A0" },
        { 11, 1, "A4" },
        { 12, 3, "A1" },
        { 13, 1, "A7" },
        { 14, 0, ""   },
        { 15, 1, "B0" },
        { 16, 3, "B5" },
        { 17, 0, ""   },
        { 18, 3, "B6" },
        { 19, 1, "B2" },
        { 20, 0, ""   },
        { 21, 1, "B1" },
        { 22, 1, "A2" },
        { 23, 1, "B3" },
        { 24, 1, "B4" },
        { 25, 0, ""   },
        { 26, 1, "B5" },
        { 27, 1, "B7" },
        { 28, 1, "B6" },
        { 29, 1, "D7" },
        { 30, 0, ""   },
        { 31, 3, "A0" },
        { 32, 1, "A3" },
        { 33, 3, "C2" },
        { 34, 0, ""   },
        { 35, 3, "A2" },
        { 36, 3, "A5" },
        { 37, 3, "C1" },
        { 38, 3, "A4" },
        { 39, 0, ""   },
        { 40, 3, "A3" }
    };

    static const pchar names_chips[4] =
    {
        "gpiochip0",
        "gpiochip1",
        "gpiochip2",
        "gpiochip3"
    };
}


InputPinInfo PinInfo::GetInputPinInfo(Pin::E pin_e)
{
    InputPinInfo result;

    if (std::strlen(pins[pin_e].G) != 2)
    {
        return result;
    }

    const Info &pin = pins[pin_e];

    result.hw.pin_number = (pin.G[0] - 'A') * 8 + pin.G[1] - '0';

    result.hw.chip_name = names_chips[pin.num_chip];

    return result;
}


OutputPinInfo PinInfo::GetOutputPinInfo(Pin::E pin_e)
{
    OutputPinInfo result;

    if (std::strlen(pins[pin_e].G) != 2)
    {
        return result;
    }

    const Info &pin = pins[pin_e];

    result.hw.pin_number = (pin.G[0] - 'A') * 8 + pin.G[1] - '0';

    result.hw.chip_name = names_chips[pin.num_chip];

    return result;
}
