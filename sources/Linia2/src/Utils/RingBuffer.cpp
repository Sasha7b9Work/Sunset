// 2025/10/23 11:56:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/RingBuffer.h"


void RingBuffer::Push(char byte)
{
    buffer[in] = byte;

    if (++in == SIZE)
    {
        in = 0;
    }
}


char RingBuffer::Pop()
{
    char result = buffer[out];

    if (++out == SIZE)
    {
        out = 0;
    }

    return result;
}


bool RingBuffer::IsEmpty() const
{
    return in == out;
}
