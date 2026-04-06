#pragma once
#include "Utils/StringUtils.h"


template<int size_buffer>
class Buffer
{
public:

    Buffer() : size(0) {}; //-V730

    // Удвалить первые n байт
    void RemoveFirst(int n)
    {
        if (n >= size)
        {
            Clear();
        }
        else
        {
            size -= n;

            uint8 temp[size_buffer];

            std::memcpy(temp, buffer, size_buffer);

            std::memset(buffer, 0xFF, size_buffer);

            std::memmove(buffer, temp + n, (uint)size);
        }
    }

    uint8 *Data() { return buffer; }

    const uint8 *DataConst() const { return buffer; }

    char *DataChar() { return (char *)buffer; }

    // Возвращает количество элементов в буфере
    int Size() const
    {
        return size;
    }

    bool Append(uint8 elem)
    {
        if (size == size_buffer)
        {
            return false;
        }

        buffer[size++] = elem;

        return true;
    }

    bool Append(const void *data, int _size)
    {
        if (Size() + _size > size_buffer)
        {
            return false;
        }

        std::memcpy(&buffer[size], data, (uint)_size);
        size += _size;

        return true;
    }

    void Clear()
    {
        size = 0;

        std::memset(buffer, 0xFF, size_buffer);
    }

    // Возвращает позицию первого элемента последовательности data в buffer, если таковая имеется. Иначе : -1.
    int Position(const void *data, int num_bytes) const
    {
        if (num_bytes > size)
        {
            return -1;
        }

        for (int i = 0; i <= size - num_bytes; i++)
        {
            void *pointer = (void *)&buffer[i];

            if (std::memcmp(pointer, data, (uint)num_bytes) == 0)
            {
                return i;
            }
        }

        return -1;
    }

    uint8 &operator[](int i)
    {
        if (i >= 0 && i < Size())
        {
            return buffer[i];
        }

        static uint8 null(0);

        return null;
    }

    operator char *()
    {
        return DataChar();
    }

protected:

    int size;

    uint8 buffer[size_buffer];
};


class Buffer1024 : public Buffer<1024>
{
public:
    Buffer1024() : Buffer<1024>() { };
};


class ParserBuffer : public Buffer1024
{
public:
    ParserBuffer() : Buffer1024() { }

    ParserBuffer(pchar string) : Buffer1024()
    {
        std::strcpy((char *)buffer, string);
    }

    bool FirstBytesIs(pchar symbols)
    {
        return std::memcmp(Data(), symbols, std::strlen(symbols)) == 0;
    }

    // Слово nuw_word == word
    bool WordIs(int num_word, pchar string);

    // Возвращает int из слова num_word
    int GetINT(int num_word)
    {
        char word[32];

        SU::GetWord(*this, num_word, word);

        return atoi(word);
    }

    // Берёт значение из 16-ричной строки
    uint GetUINTFromHEX(int num_word);
};
