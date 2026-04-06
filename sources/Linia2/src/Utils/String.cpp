// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Buffer.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"


pchar  const String::_ERROR = "---.---";


String::String() : buffer(nullptr)
{
    Set("");
}


String::String(const String &rhs) : buffer(nullptr)
{
    Set("");

    Allocate(static_cast<int>(std::strlen(rhs.c_str()) + 1));

    if(buffer != nullptr)
    {
        std::strcpy(buffer, rhs.c_str());
    }
}


String::String(char symbol) : buffer(nullptr)
{
    Set("");

    Allocate(2);

    if(buffer != nullptr)
    {
        buffer[0] = symbol;
        buffer[1] = 0;
    }
}


String::String(pchar format, ...) : buffer(nullptr)
{
    Free();

    std::va_list args;
    va_start(args, format);
    int sizeBuffer = std::vsnprintf(nullptr, 0, format, args) + 1;
    va_end(args);

    Allocate(sizeBuffer);

    std::va_list args1;
    va_start(args1, format);
    std::vsnprintf(buffer, (uint)(sizeBuffer), format, args1);
    va_end(args1);
}


void String::Set(pchar value)
{
    Free();

    Allocate((int)(std::strlen(value) + 1));

    if (buffer)
    {
        std::strcpy(buffer, value);
    }
}


void String::Append(pchar str)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    Allocate(static_cast<int>(old.Size() + std::strlen(str) + 1));

    std::strcpy(buffer, old.c_str());
    std::strcat(buffer, str);
}


void String::Append(pchar str, int numSymbols)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    int size = numSymbols + old.Size() + 1;

    Allocate(size);

    std::strcpy(buffer, old.c_str());
    std::memcpy(buffer + old.Size(), str, static_cast<uint>(numSymbols));
    buffer[size - 1] = '\0';
}


void String::Append(char symbol)
{
    char b[2] = { symbol, '\0' };
    Append(b);
}


String::~String()
{
    std::free(buffer);
}


void String::Free()
{
    if(buffer)
    {
        std::free(buffer);
        buffer = nullptr;
        Set("");
    }
}


char *String::c_str() const
{
    return buffer;
}


void String::Allocate(int size)
{
    std::free(buffer);

    buffer = static_cast<char *>(std::malloc(static_cast<uint>(size + 1)));
}


void String::RemoveFromBegin(int numSymbols)
{
    if (std::strlen(buffer) == static_cast<uint>(numSymbols))
    {
        Free();
    }
    else
    {
        String old(buffer);

        Free();

        Allocate(old.Size() - numSymbols + 1);

        std::strcpy(buffer, old.c_str() + numSymbols);
    }
}


void String::RemoveFromEnd()
{
    if(Size() > 0)
    {
        buffer[Size() - 1] = '\0';
    }
}


int String::Size() const
{
    if (buffer == nullptr)
    {
        return 0;
    }

    return static_cast<int>(std::strlen(buffer));
}


char &String::operator[](int i) const
{
    static char result = 0;

    if (buffer == nullptr || Size() < i)
    {
        return result;
    }

    return buffer[i];
}


bool String::operator!=(pchar rhs) const
{
    return std::strcmp(buffer, rhs) != 0;
}


bool String::operator==(pchar rhs) const
{
    return std::strcmp(buffer, rhs) == 0;
}


String String::GetWord(int num, pchar delimit) const
{
    char out[1024];

    return String(SU::GetWord(buffer, num, out, delimit));
}


bool String::BeginWith(pchar symbols) const
{
    if (std::strlen(buffer) >= std::strlen(symbols))
    {
        return std::memcmp(buffer, symbols, std::strlen(symbols)) == 0;
    }

    return false;
}


String String::GetLastWord(pchar delimit) const
{
    int num_words = SU::GetNumWords(buffer, delimit);

    char out[1024];

    return String(SU::GetWord(buffer, num_words, out, delimit));
}


void String::ToUpper()
{
    char *p = buffer;

    while (*p)
    {
        *p = (char)std::toupper(*p);
        p++;
    }
}
