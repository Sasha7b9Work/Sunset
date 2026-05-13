// 2025/10/23 11:44:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/SCPI/SCPI.h"
#include "Utils/RingBuffer.h"


namespace SCPI
{
    static RingBuffer ring_buffer;

    class BufferSCPI
    {
    public:
        void Push(uint8);
        bool PeriodicTask();
    private:
        static const int SIZE = 1024;
        uint8 buffer[SIZE];
        int pointer = 0;
        bool ExistMessage();
        pchar GetMessage();
        void RemoveMessage();
        // Возвращает первую встреченную позицию символа
        int FindSymbol(char) const;
        void Clear();
        int Size() const;
    } buffer;

    extern StructSCPI head[];

    static bool PeriodicTask(pchar, StructSCPI *);
}


void SCPI::OnEventCallback(uint8 *bytes, int size)
{
    for (int i = 0; i < size; i++)
    {
        uint8 byte = bytes[i];

        char symbol = (char)byte;

        if ((byte & 0x80) == 0)
        {
            symbol = (char)std::toupper((int)byte);
        }
        else
        {
            symbol &= 0x7F;
        }

        ring_buffer.Push(symbol);
    }
}


bool SCPI::PeriodicTask()
{
    while (!ring_buffer.IsEmpty())
    {
        buffer.Push((uint8)ring_buffer.Pop());
    }

    bool result = false;

    while (buffer.PeriodicTask())
    {
        result = true;
    }

    return result;
}


void SCPI::BufferSCPI::Push(uint8 byte)
{
    buffer[pointer++] = byte;
}


bool SCPI::BufferSCPI::PeriodicTask()
{
    if (ExistMessage())
    {
        pchar message = GetMessage();

        bool result = SCPI::PeriodicTask(message, SCPI::head);

        if (!result)
        {
            LOG_ERROR("Error process message %s", message); //-V111
        }

        RemoveMessage();

        return result;
    }

    return false;
}


bool SCPI::BufferSCPI::ExistMessage()
{
    if (Size() == 0)
    {
        return false;
    }

    int pos = FindSymbol(':');

    if (pos < 0)
    {
        Clear();

        return false;
    }

    if (pos > 0)
    {
        std::memmove(buffer, buffer + pos, (size_t)(Size() - pos)); //-V201
        pointer -= pos;
    }

    return FindSymbol('\0') > 0;
}


pchar SCPI::BufferSCPI::GetMessage()
{
    return (pchar)buffer;
}


int SCPI::BufferSCPI::FindSymbol(char symbol) const
{
    for (int i = 0; i < pointer; i++)
    {
        if ((char)buffer[i] == symbol)
        {
            return i;
        }
    }

    return -1;
}


void SCPI::BufferSCPI::Clear()
{
    pointer = 0;
}


int SCPI::BufferSCPI::Size() const
{
    return pointer;
}


void SCPI::BufferSCPI::RemoveMessage()
{
    int pos = FindSymbol('\0');

    if (pos >= 0)
    {
        std::memmove(buffer, buffer + pos + 1, (size_t)pointer - pos + 1); //-V201
        pointer = pointer - pos - 1;
    }
}


bool SCPI::PeriodicTask(pchar message, StructSCPI *handler)
{
    while (*message == ':' || *message == ' ')
    {
        message++;
    }

    while (handler->begin != nullptr)
    {
        size_t len_begin = std::strlen(handler->begin);

        if (std::strlen(message) >= std::strlen(handler->begin))
        {
            if (std::memcmp(handler->begin, message, len_begin) == 0)
            {
                if (handler->func)
                {
                    message += len_begin;

                    while (*message == ':' || *message == ' ')
                    {
                        message++;
                    }

                    return handler->func(message);
                }
                else if (handler->handler)
                {
                    message += len_begin;

                    return PeriodicTask(message, handler->handler);
                }
            }
        }

        handler++;
    }

    return false;
}