// 2025/10/15 11:19:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/RingBuffer.h"


void RingBuffer::Append(uint8 byte)
{
    buffer[in_index] = byte;
    in_index++;
    if (in_index == CAPACITY)
    {
        in_index = 0;
    }
}


bool RingBuffer::IsEmpty() const
{
    return in_index == out_index;
}


uint8 RingBuffer::Pop()
{
    if (IsEmpty())
    {
        return 0;
    }

    uint8 result = buffer[out_index];

    out_index++;
    if (out_index == CAPACITY)
    {
        out_index = 0;
    }

    return result;
}
