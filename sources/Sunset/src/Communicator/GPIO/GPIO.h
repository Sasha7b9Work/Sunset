#pragma once
#include "Communicator/GPIO/PinInfo.h"


class PinIn : public Pin
{
public:

    using Pin::Pin;

    using ChangeCallback = std::function<void(bool)>;

    void SetChangeCallback(ChangeCallback);

    // Время чтения пина - 0.5 мкс
    static bool GetHardware(gpiod_line *);

private:

    ChangeCallback callback_;
};


class PinOut : public Pin
{
public:

    using Pin::Pin;

    void Set(bool state);

    void ToLow();

    void ToHi();

    // Время установки пина - 0.5 мкс
    static void Set(gpiod_line *, int);
};

namespace GPIO
{
    void Init();
    void DeInit();

    InputPinInfo *GetInputPinInfo(Pin::E);
    OutputPinInfo *GetOutputPinInfo(Pin::E);
}


extern PinIn pinDAT_F0;
extern PinIn pinDAT_F2;
extern PinIn pinFIFO_FULL;

extern PinIn pinSTART;
extern PinIn pinSTOP;
extern PinIn pinKA;
extern PinIn pinKB;

extern PinOut pinREQ_RD;
extern PinOut pinOut;
