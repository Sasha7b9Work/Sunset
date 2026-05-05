// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/StringUtils.h"
#include "Utils/Buffer.h"

#ifndef LANG
#define LANG 0
#define LANG_RU true
#endif


bool String2Int(char *str, int *value)
{
    int sign = str[0] == '-' ? -1 : 1;
    if (str[0] < '0' || str[0] > '9')
    {
        str++;
    }
    uint64 length = std::strlen(str);
    if (length == 0)
    {
        return false;
    }

    *value = 0;
    int pow = 1;
    uint64 i = length;
    do
    {
        --i;
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *value += val * pow;
        pow *= 10;
    } while (i > 0);

    if (sign == -1)
    {
        *value *= -1;
    }
    return true;
}


int BCD2Int(uint bcd)
{
    uint pow = 1;

    int value = 0;

    for (int i = 0; i < 8; i++)
    {
        value += (bcd & 0x0f) * pow;
        pow *= 10;
        bcd = bcd >> 4;
    }

    return value;
}


#define  SYMBOL(x) (*(*(x)))


#ifndef WIN32

int strcpy_s(char *dest, uint dest_size, pchar src)
{
    if (std::strlen(src) + 1 < dest_size)
    {
        std::strcpy(dest, src);
        return 0;
    }

    return 1;
}

#endif



char *SU::GetWord(pchar data, int num, char out[32], pchar delimit)
{
    Words words(data, delimit);

    if (num <= words.Size())
    {
        std::strcpy(out, words.At(num - 1).c_str());
    }
    else
    {
        out[0] = '\0';
    }

    return out;
}


int SU::GetNumWords(pchar data, pchar delimit)
{
    Words words(data, delimit);

    return words.Size();
}


void SU::SplitToWords(const wxString &str, Words &words, pchar delimiters)
{
    words.Clear();

    char buf[1024];

    std::strcpy(buf, str.c_str());

    char *token = std::strtok(buf, delimiters);

    while (token != nullptr)
    {
        words.Push(token);
        token = std::strtok(nullptr, delimiters);
    }
}


void SU::SplitToParameters(pchar message, Parameters &parameters, pchar delimit)
{
    parameters.Clear();

    Words words(message, delimit);

    for (int i = 0; i < words.Size(); i++)
    {
        std::string string = words.At(i);

        uint64 index = string.find('=');

        if(index != std::string::npos)
        {
            Parameter parameter({ string.substr(0, index), string.substr(index + 1) });

            parameters.parameters.push_back(parameter);
        }
    }
}


uint64 SU::UInt64FromHex(const wxString &value)
{
    unsigned long long result = 0;

    value.ToULongLong(&result, 16);

    return (uint64)result;
}


std::string Parameters::Find(pchar key)
{
    for (Parameter param : parameters)
    {
        if (param.key == key)
        {
            return param.value;
        }
    }

    return "";
}


int Parameters::FindInt(pchar key)
{
    std::string param = Find(key);

    if (param.length())
    {
        return std::atoi(param.c_str());
    }

    return -1;
}


pchar SU::Consist(pchar word, pchar sub_word)
{
    return std::strstr(word, sub_word);
}


bool SU::BeginWith(pchar string, pchar sub_word)
{
    if (std::strlen(string) >= std::strlen(sub_word))
    {
        return std::memcmp(string, sub_word, std::strlen(sub_word)) == 0;
    }

    return false;
}


uint SU::UIntFromString(pchar string)
{
    char *end = nullptr;

    return (uint)std::strtoul(string, &end, 10);
}


uint64 SU::UInt64FromString(pchar string)
{
    char *end = nullptr;

    return std::strtoull(string, &end, 10);
}


bool Words::Consist(const wxString &word) const
{
    for (auto str : words)
    {
        if (str == word)
        {
            return true;
        }
    }

    return false;
}


int Words::GetArrayElementCount() const
{
    return (int)words.GetCount();
}


std::string Words::At(int i) const
{
    if (i < Size())
    {
        return std::string(words[(uint)i].mb_str());
    }

    static const std::string null;
    return null;
}


const wxString &Words::Last() const
{
    if (Size())
    {
        return words[(uint)Size() - 1];
    }

    LOG_WRITE("Empty vector");
    static const wxString null;
    return null;
}


const wxString &Words::First() const
{
    if (Size())
    {
        return words[0];
    }

    static const wxString null;
    return null;
}


double SU::StringToDouble(const wxString &str)
{
    wxString value = str.BeforeFirst(' ');
    wxString units = str.AfterFirst(' ');

    int int_value = 0;
    value.ToInt(&int_value);

    double multiplier = 1.0;

    if (units[0] == 'm')        multiplier = 1e-3;
    else if (units[0] == 'u')   multiplier = 1e-6;
    else if (units[0] == 'n')   multiplier = 1e-9;
    else if (units[0] == 'p')   multiplier = 1e-12;
    else if (units[0] == 'k')   multiplier = 1e3;

    return int_value * multiplier;
}


wxString SU::BinToString(uint value, int bit_deth)
{
    wxString result;

    for (int i = 0; i < bit_deth; i++)
    {
        int bit = (int)(value & (1 << i));

        result.Prepend(bit == 0 ? '0' : '1');
    }

    return result;
}


double SU::ASCIItoFloat(pchar str)
{
    return std::atof(str);
}
