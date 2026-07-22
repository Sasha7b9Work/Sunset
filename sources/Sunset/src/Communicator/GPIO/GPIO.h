#pragma once


// Структуры для аппаратной информации GPIO (нужны для Linux)
struct gpiod_chip;
struct gpiod_line;


struct HardwarePinInfo
{
    int pin_number;          // Номер GPIO пина чипа
    const char *chip_name;   // Имя GPIO чипа
    gpiod_chip *chip;        // Дескриптор чипа
    gpiod_line *line;        // Дескриптор линии
    pchar name_connector;    // Так он обозначен на разъёме
};


struct InputPinInfo
{
    HardwarePinInfo hw;
    bool last_state;                    // Последнее состояние
    std::function<void(bool)> callback; // Callback функция
};


struct OutputPinInfo
{
    HardwarePinInfo hw;
};


/*
          Стало     Было          Стало    Было
                             21 SPI_MISO
                             22 F_CON2
                             23 SPI_CLK
                             24 SPICS
                             25 GND
     6 GND                   26 F_CON1
                             27 ENB_PC
     8 TX                    28 T18
     9 GND                   29 T16
    10 RX                    30 GND
                             31 EN_DDA1
    12 T17                   32 ST_EXT
                             33 K_STOP
    14 GND                   34 GND
    15 K_START               35 EN_DDA2
    16 DT_DDAC               36 FULL
    17                       37 FIT
    18 CLK_DDAC              38
    19                       39 GND
    20 GND                   40
*/


class Pin
{
public:
    enum E
    {                                  //   Tребуется изменение
        In_START,       // 15 Кнопка СТАРТ
        In_STOP,        // 21 Кнопка СТОП    +
        In_DAT_F0,      // 16                +
        In_SPI_MOSI,    // 18                +
        In_DAT_F2,      // 22
        Out_SPI_CS,     // 24                +
        In_FIFO_FULL,   // 36
        Out_REQ_RD,     // 32
        In_KA,          // 11 GPIO1_A4
        In_KB,          // 13 GPIO1_A7
        Out,            // 26 GPIO1_B5
        Count
    };
    explicit Pin(E type) : type_(type) { }

    E type() const
    {
        return type_;
    }

    bool Get() const;

protected:
    E type_;
};


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
extern PinIn pinSPI_MOSI;
extern PinIn pinDAT_F2;
extern PinIn pinFIFO_FULL;

extern PinIn pinSTART;
extern PinIn pinSTOP;
extern PinIn pinKA;
extern PinIn pinKB;

extern PinOut pinREQ_RD;
extern PinOut pinOut;
extern PinOut pinSPI_CS;
