// 2025/09/10 21:36:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/I_IPPP.h"


class EmulatorIPPP : public I_IPPP
{
public:

    virtual ~EmulatorIPPP() override {}

    void PeriodicTask() override;

    void PressButtonStart() override;

    void PressButtonStop() override;

private:

    bool in_process_measuring = false;                  // Устанавливается при нажатии кнопки СТАРТ. Снимается при нажатии кнопки СТОП.

    virtual bool ReadData(int data1[5][MAX_NUMBER_POINTS]) override;
};
