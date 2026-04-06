// 2025/10/23 11:49:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer.h"


class RingBuffer
{
public:
    void Push(char);
    char Pop();
    bool IsEmpty() const;
private:
    static const int SIZE = 1024;
    char buffer[SIZE];
    int in = 0;                 // Сюда ложим очередной байт
    int out = 0;                // А отсюда считываем
};
