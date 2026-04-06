// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"


struct Parameter
{
    std::string key;
    std::string value;
};


struct Parameters
{
    std::vector<Parameter> parameters;

    std::string Find(pchar key);

    int FindInt(pchar key);

    void Clear() { parameters.clear(); }
};


struct Words
{
    Words(const wxString &line, pchar delimiters = " ")
    {
        words = wxStringTokenize(line, delimiters);
    }

    int Size() const
    {
        return (int)words.size();
    }
    void Clear()
    {
        words.clear();
    }
    void Push(const std::string &word)
    {
        words.push_back(word);
    }
    std::string At(int i) const;
    wxString operator[](int i) const
    {
        return At(i);
    }
    const wxString &Last() const;
    const wxString &First() const;

    // true, если содержит слово
    bool Consist(const wxString &) const;

private:

    wxArrayString words;
};


namespace SU
{
    wxString BinToString(uint value, int bit_deth);

    // Переводит в double строки типа "20 nA", "2 kV"
    double StringToDouble(const wxString &);

    // Возвращает указатель на первое вхождение sub_word в word. nullptr, если не обнаружено
    pchar Consist(pchar word, pchar sub_word);

    // Возвращает true, если string начинается с sub_word
    bool BeginWith(pchar string, pchar sub_word);

    void SplitToWords(const wxString &, Words &, pchar delimit = " \n");

    void SplitToParameters(pchar, Parameters &, pchar delimit = "\n");

    uint64 UInt64FromHex(const wxString &);

    // Нумерация начинается с 1
    char *GetWord(pchar data, int num, char out[32], pchar delimit = " \n\0");

    int GetNumWords(pchar data, pchar delimit = " \n");

    uint UIntFromString(pchar string);

    uint64 UInt64FromString(pchar string);

    // Оставить number последних символов от in. Если строка меньше, то в начале добавить пробелы
    pchar LeaveTheLastOnes(pchar in, int number);
};
