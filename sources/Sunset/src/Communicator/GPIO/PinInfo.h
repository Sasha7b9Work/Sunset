// 2026/07/23 16:19:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
          Стало     Было          Стало    Было
     1                       21 SPI_MISO   STOP
     2                       22 F_CON2     DAT_F2
     3 T13                   23 SPI_CLK    SPI1_CLK
     4                       24 SPICS      DAT_F3
     5 T14                   25 GND
     6 GND                   26 F_CON1     SCICS
     7 T15                   27 ENB_PC     ENB_STM
     8 TX                    28 T18
     9 GND        -          29 T16
    10 RX                    30 GND
    11 KA                    31 EN_DDA1
    12 T17                   32 ST_EXT    REQ_RD
    13 KB                    33 K_STOP    -
    14 GND                   34 GND
    15 K_START               35 EN_DDA2
    16 DT_DDAC  DAT_F0       36 FULL
    17                       37 FIT       -
    18 CLK_DDAC DAT_F1       38
    19          SPI1_MOSI    39 GND
    20 GND      GNDK         40          RDY


       Замены

       Было       Стало
    16 DAT_F0     DT_DDAC
    18 DAT_F1     CLK_DDAC
    22 DAT_F2     F_CON2
    26 SCICS      F_CON1
    27 ENB_STM    ENB_PC
    32 REQ_RD     ST_EXT
    33 -          KN_STOP
    37 -          FIT
    40 RDY        -

        Сделано
    19 SPI1_MOSI  -
    21 STOP       SPI_MISO
    23 SPI1_CLK   SPI_CLK
    24 DAT_F3     SPICS

    SPI0_M2:
    21 MISO
    23 CLK
    24 CS0
*/


class Pin
{
public:
    enum E
    {
        _00,
        _01,
        _02,
        T13_03,
        _04,
        T14_05,
        _06,
        T15_07,
        _08,
        _09,
        _10,
        KN_A_11,
        T17_12,
        KN_B_13,
        _14,
        KN_START_15,
        DT_DDAC_16_out,
        _17,
        CLK_DDAC_18_out,
        _19,
        _20,
        _21,
        F_CON2_22,
        _23,
        _24,
        _25,
        F_CON1_26,
        ENB_PC_27,
        _28,
        _29,
        _30,
        EN_DDA1_31_out,
        ST_EXT_32,
        KN_STOP_33,
        _34,
        EN_DDA2_35_out,
        FULL_36,
        FIT_37,
        _38,
        _39,
        _40,
        Count
    };

    explicit Pin(E type) : type_(type) {}

    E type() const
    {
        return type_;
    }

    bool Get() const;

protected:
    E type_;
};


// Структуры для аппаратной информации GPIO (нужны для Linux)
struct gpiod_chip;
struct gpiod_line;


struct HardwarePinInfo
{
    int pin_number;                     // Номер GPIO пина чипа
    const char *chip_name = nullptr;    // Имя GPIO чипа
    gpiod_chip *chip = nullptr;         // Дескриптор чипа
    gpiod_line *line = nullptr;         // Дескриптор линии
};


struct OutputPinInfo
{
    HardwarePinInfo hw;
};


struct InputPinInfo
{
    HardwarePinInfo hw;
    bool last_state = false;                        // Последнее состояние
    std::function<void(bool)> callback = nullptr;   // Callback функция
};


namespace PinStorage
{
    InputPinInfo GetInputPinInfo(Pin::E);

    OutputPinInfo GetOutputPinInfo(Pin::E);
}
