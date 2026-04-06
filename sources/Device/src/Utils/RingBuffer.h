// 2025/10/15 11:13:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class RingBuffer
{
public:
    void Append(uint8);
    bool IsEmpty() const;
    uint8 Pop();
private:
    static const int CAPACITY = 1024;
    uint8 buffer[CAPACITY];
    int in_index = 0;           // —юда будет записан следующий символ
    int out_index = 0;          // ќтсюда будет считан следующий символ
};
