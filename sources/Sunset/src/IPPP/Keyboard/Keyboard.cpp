// 2025/09/11 21:22:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Keyboard/Keyboard.h"
#include "Communicator/GPIO/GPIO.h"
#include "Utils/Timer.h"
#include "Application.h"


namespace Keyboard
{
#define PIN_START pins[0]
#define PIN_STOP  pins[1]
#define PIN_KA    pins[2]
#define PIN_KB    pins[3]

    static const int64 TIME_EVENT_BTN = 100;    // Столько мс состояние кнопки не должно меняться, чтобы действие свершилось (пропустить дребезг контактов)
    static const int64 TIME_EVENT_GOV = 3;

//    static void CallbackOnSTART(bool);
//    static void CallbackOnSTOP(bool);
//    static void CallbackOnKA(bool);
//    static void CallbackOnKB(bool);

    struct StructPin
    {
        int64  event_time = 0;     // Время предыдущего изменения состояния. Если 0, то изменения не было
        PinIn *pin = nullptr;
        bool   press = false;      // Предыдущее состояние. true - была нажата (переход из 0 в 1), false - была отпущена (переход из 1 в 0)
    };

    static StructPin pins[4];
}


void Keyboard::Init()
{
//    pinSTART.SetChangeCallback(CallbackOnSTART);
//
//    pinSTOP.SetChangeCallback(CallbackOnSTOP);
//
//    pinKA.SetChangeCallback(CallbackOnKA);
//
//    pinKB.SetChangeCallback(CallbackOnKB);

    for (int i = 0; i < 4; ++i)
    {
        pins[i] = StructPin{};
    }

    PIN_START.pin = &pinSTART;
    PIN_STOP.pin = &pinSTOP;
    PIN_KA.pin = &pinKA;
    PIN_KB.pin = &pinKB;
}


void Keyboard::PeriodicTask()
{
    int64 time = Timer::CurrentTimeMS();

    if (PIN_START.event_time)                                     // Идёт событие - нажатие или отпускание
    {
        if (time - PIN_START.event_time > TIME_EVENT_BTN)         // Если после последнего события прошло достаточно времени
        {
            TheApp->OnButtonStart(PIN_START.press);    // То считаем, что кнопка в устойчивом положении - обрабатываем нажатие
            PIN_START.event_time = 0;                             // И устанавливаем признак того, что событие произошло
        }
    }

    if (PIN_STOP.event_time)
    {
        if (time - PIN_STOP.event_time > TIME_EVENT_BTN)
        {
            TheApp->OnButtonStop(PIN_STOP.press);
            PIN_STOP.event_time = 0;
        }
    }

    if (PIN_KA.event_time && PIN_KB.event_time)
    {
        if ((time - PIN_KA.event_time) > TIME_EVENT_GOV &&
            (time - PIN_KB.event_time) > TIME_EVENT_GOV)
        {
            if (PIN_KA.press && !PIN_KB.press)
            {
                TheApp->OnGovernor(PIN_KA.event_time > PIN_KB.event_time);
            }
            else if (!PIN_KA.press && PIN_KB.press)
            {
                TheApp->OnGovernor(PIN_KA.event_time > PIN_KB.event_time);
            }

            PIN_KA.event_time = 0;
            PIN_KB.event_time = 0;
        }
    }
}


//void Keyboard::CallbackOnSTART(bool press)
//{
//    PIN_START.press = press;
//    PIN_START.event_time = Timer::CurrentTimeMS();
//}


//void Keyboard::CallbackOnSTOP(bool press)
//{
//    PIN_STOP.press = press;
//    PIN_STOP.event_time = Timer::CurrentTimeMS();
//}


//void Keyboard::CallbackOnKA(bool press)
//{
//    PIN_KA.press = press;
//    PIN_KA.event_time = Timer::CurrentTimeMS();
//}


//void Keyboard::CallbackOnKB(bool press)
//{
//    PIN_KB.press = press;
//    PIN_KB.event_time = Timer::CurrentTimeMS();
//}
