// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"
#include <cstdarg>


class String // -V690
{
public:
    explicit String();
    String(const String &);
    explicit String(char symbol);
    String(pchar format, ...);
    ~String();

    void Set(pchar value);

    char *c_str() const;

    static pString _ERROR;

    void Free();

    void Append(pchar str);

    void Append(pchar str, int numSymbols);

    void Append(char symbol);

    // Удаляет numSymbols из начала строки
    void RemoveFromBegin(int numSymbols);

    void RemoveFromEnd();

    int Size() const;

    // Нумерация начинается с 1
    String GetWord(int num, pchar delimit) const;

    String GetLastWord(pchar delimit) const;

    void ToUpper();

    bool BeginWith(pchar) const;

    char &operator[](int i) const;

    bool operator!=(pchar) const;

    bool operator==(pchar) const;

private:

    void Allocate(int size);

    char *buffer;

    void ParseArguments(pchar format, std::va_list args);
};
