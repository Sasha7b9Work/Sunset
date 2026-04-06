// 2025/10/23 11:43:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
struct StructSCPI
{
    pchar begin;            // С этим сравниваем начало передаваемой строки
    bool (*func)(pchar);    // А эта функция вызывается в случае, если начало совпадает
    StructSCPI *handler;    // Или передаётся в эту последовательность структур
};


namespace SCPI
{
    void OnEventCallback(uint8 *, int);

    bool PeriodicTask();
}
