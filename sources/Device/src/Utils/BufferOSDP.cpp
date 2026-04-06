// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/BufferOSDP.h"
#include <cstdlib>
#include <cstring>


BufferOSDP::BufferOSDP(int new_capacity) : buffer(nullptr), size(0)
{
    Allocate(new_capacity);
}


BufferOSDP::~BufferOSDP()
{
    Free();
}


void BufferOSDP::Free()
{
    std::free(buffer);
    size = 0;
    capacity = 0;
}


void BufferOSDP::Allocate(int new_capacity)
{
    char *temp = nullptr;

    if (size)
    {
        temp = (char *)std::malloc((uint)capacity);
        if (temp)
        {
            std::memcpy(temp, buffer, (uint)capacity);
        }
    }

    std::free(buffer);

    buffer = (char *)std::malloc((uint)new_capacity);
    if (buffer && temp)
    {
        std::memset(buffer, 0, (uint)new_capacity);
        std::memcpy(buffer, temp, (uint)capacity);
    }
    capacity = new_capacity;

    std::free(temp);
}


void BufferOSDP::Append(char byte)
{
    if (IsFull())
    {
        Allocate(capacity + 1024);
    }

    buffer[size++] = byte;
}


bool BufferOSDP::IsFull() const
{
    return (size == capacity);
}


bool BufferOSDP::IsEmpty() const
{
    return size == 0;
}


const char &BufferOSDP::operator[](int num) const
{
    return buffer[num];
}


void BufferOSDP::RemoveFirst(int num_bytes)
{
    if (num_bytes >= size)
    {
        size = 0;
    }
    else
    {
        std::memmove(buffer, buffer + num_bytes, (uint)(size - num_bytes));
        size -= num_bytes;
    }
}


int BufferOSDP::FirstPosition(char symbol)
{
    for (int i = 0; i < size; i++)
    {
        if (buffer[i] == symbol)
        {
            return i;
        }
    }

    return -1;
}


pchar BufferOSDP::Data(int pos) const
{
    return buffer + pos;
}
