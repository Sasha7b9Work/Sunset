// 2025/08/30 18:38:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Port
{
    enum E
    {
        _A,
        _B,
        _C,
        _D,
        _E,
        _F,
        _G,
        _H,
        _I,
        Count
    };
};


struct PinMode
{
    enum E
    {
        Out,
        In,
        Count
    };
};

class Pin
{
public:

    enum E
    {
        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        _10,
        _11,
        _12,
        _13,
        _14,
        _15,
        Count
    };

    Pin(Port::E, Pin::E, PinMode::E);

    virtual ~Pin() { }

    virtual void Init() = 0;

protected:

    void      *port;
    uint16     pin;
    PinMode::E mode;
};


class PinIn : public Pin
{
public:
    PinIn(Port::E _port, Pin::E _pin) : Pin(_port, _pin, PinMode::In) { }

    virtual void Init() override;

    // До вызоыва Get() эта функция вернёт предыдущее состояние входа
    bool PrevState() const;

    // Эта функция изменяет состояние PrevState()
    bool Get();

private:

    bool prev_state = false;
};


class PinOut : public Pin
{
public:
    PinOut(Port::E _port, Pin::E _pin) : Pin(_port, _pin, PinMode::Out) { }
    virtual void Init() override;
    void ToLow();
    void ToHi();
    void Set(bool);
};


// Дополнительная структура для всяких надобностей
struct StructPinIn
{
    PinIn *pin;
    pchar name;
};


//------------------ Канал базы, канал подложки
extern PinOut pDAT1_DAC;        // PD8  77
extern PinOut pCLK1_DAC;        // PD11 80

//------------------ Измеритель тока, формирователь развёртки, коммутатор, источник напряжения 3кВ
extern PinOut pDAT2_DAC;        // PB15 76
extern PinOut pCLK2_DAC;        // PD9  78
