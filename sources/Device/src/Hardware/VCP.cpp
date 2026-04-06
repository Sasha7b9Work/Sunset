// 2026/01/19 15:00:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/VCP.h"
#include "Hardware/USBD/usbd_cdc_interface.h"
#include "Device/PCM/SCPI.h"


namespace VCP
{
    static BufferOSDP in_buffer(1024);

    static bool ProcessInputBuffer();
}


void VCP::PeriodicTask()
{
    _CDC::GetData(in_buffer);

    while (!in_buffer.IsEmpty() && ProcessInputBuffer())
    {
    }
}


bool VCP::ProcessInputBuffer()
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

    PCM::SCPI::Parse(in_buffer.Data(pos_begin));

    in_buffer.RemoveFirst(pos_end + 1);

    return true;
}
