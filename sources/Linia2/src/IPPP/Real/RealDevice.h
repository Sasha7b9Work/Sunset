// 2025/09/11 08:48:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/IDevice.h"


class RealDevice : public IDevice
{
public:

    RealDevice() = default;

    virtual ~RealDevice();

    virtual bool Init() override;

    virtual void PeriodicTask() override;

    virtual void Shutdown() override;

    virtual bool IsConnected() const override;

    virtual void SendCommand(pchar format, ...) const override;

private:

    std::atomic<bool> running = false;
    std::atomic<bool> connected = false;
};
