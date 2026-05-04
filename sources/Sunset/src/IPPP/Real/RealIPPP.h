// 2025/09/10 21:27:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/I_IPPP.h"
#include "Settings/Tests/SettingsTests.h"


class RealIPPP : public I_IPPP
{
public:

    virtual ~RealIPPP() override {}

    void PeriodicTask() override;

    void PressButtonStart() override;

    void PressButtonStop() override;

private:

    bool IsChanBS(const Chan &) const;

    virtual bool ReadData(int data[5][MAX_NUMBER_POINTS]) override;

    void Pause();
};
