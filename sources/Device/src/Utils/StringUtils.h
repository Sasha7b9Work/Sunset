// 2025/10/15 14:53:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace SU
{
    // true, если str начинается с begin
    bool BeginWith(pchar str, pchar begin);

    // Оставить number последних символов от in. Если строка меньше, то в начале добавить пробелы
    pchar LeaveTheLastOnes(pchar in, int number);

    // Если символ равен одному из mask
    bool CharIs(char, pchar mask);
}
