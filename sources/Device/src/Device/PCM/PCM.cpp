// 2025/09/22 13:50:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/PCM/PCM.h"
#include "Utils/BufferOSDP.h"
#include "Hardware/HAL/HAL.h"
#include "Device/PCM/SCPI.h"
#include "Hardware/HAL/HAL_PINS.h"


namespace PCM
{
    static PinOut pPI_ENB_STM(Port::_D, Pin::_12);  // PD12 81
    //Pin    pPI_TX;                                // PB6  136
    //Pin    pPI_RX;                                // PA10 102

    static BufferOSDP in_buffer(1024);

    static bool ProcessInputBuffer();

    static void UpdateInput();
}


void PCM::PeriodicTask()
{
    UpdateInput();
}


void PCM::UpdateInput()
{
    HAL_USART1::GetData(in_buffer);

    while (!in_buffer.IsEmpty() && ProcessInputBuffer())
    {
    }
}


bool PCM::ProcessInputBuffer()
{
    int pos_begin = in_buffer.FirstPosition(':');

    if (pos_begin > 0)
    {
        in_buffer.RemoveFirst(pos_begin);
    }

    int pos_end = in_buffer.FirstPosition('\0');

    if (pos_end < 0)
    {
        return false;
    }

    SCPI::Parse(in_buffer.Data(pos_begin));

    in_buffer.RemoveFirst(pos_end + 1);

    return true;
}
