// 2025/10/23 15:09:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct PinDevice
{
    enum E
    {
        _3000V,     // 300vE
        _50_P,      // 50E+
        _50_N,      // 50E-
        _200_P,     // 200E+
        _200_N,     // 200E-
        _20_P,      // 20E+
        _20_N,      // 20E-
        Count
    };

    PinDevice(E v) : value(v) { }

    pchar Name() const;

    // Это устанавливает состояние входа в данной программе - ничего не посылается в stm32
    void SetState(bool);

protected:
    E value;
};


struct PinInDevice : public PinDevice
{
    PinInDevice(PinDevice::E v) : PinDevice(v) { }
};


extern PinInDevice pin3000V;
extern PinInDevice pin50_P;
extern PinInDevice pin50_N;
extern PinInDevice pin200_P;
extern PinInDevice pin200_N;
extern PinInDevice pin20_P;
extern PinInDevice pin20_N;


extern PinInDevice *pinsD[];
