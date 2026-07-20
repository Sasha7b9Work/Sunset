// 2025/10/15 14:53:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/StringUtils.h"
#include <cstring>


bool SU::BeginWith(pchar str, pchar begin)
{
    uint length_begin = std::strlen(begin);

    if (std::strlen(str) < std::strlen(begin))
    {
        return false;
    }

    return std::memcmp((void *)str, begin, length_begin) == 0;
}


pchar SU::LeaveTheLastOnes(pchar in, int number)
{
    int size = (int)std::strlen(in);

    if (size >= number)
    {
        return in + size - number;
    }

    static char buffer[1024];

    char *pointer = buffer;

    int num_spaces = number - size;

    for (int i = 0; i < num_spaces; i++)
    {
        *pointer++ = ' ';
    }

    *pointer = '\0';

    std::strcat(buffer, in);

    return buffer;
}


bool SU::CharIs(char symbol, pchar mask)
{
    if (symbol == 0x00)
    {
        return true;
    }

    for (uint i = 0; i < std::strlen(mask); i++)
    {
        if (symbol == mask[i])
        {
            return true;
        }
    }

    return false;
}
