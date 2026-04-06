// 2025/10/23 15:26:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SoftTests/SoftTests.h"
#include "IPPP/SCPI/SCPI.h"


namespace SCPITests
{
    static const pchar commands[] =
    {
        ":PININ:3000V:STATE 0",
        ":PININ:3000V:STATE 1",
        nullptr
    };
}


bool SCPITests::RunAll()
{
    for (int i = 0; ; i++)
    {
        if (commands[i] == nullptr)
        {
            LOG_WRITE("SCPI tests is succseefull");

            return true;
        }

        pchar command = commands[i];

        while (*command) 
        {
            SCPI::OnEventCallback((uint8 *)command, 1);
            command++;
        }

        SCPI::OnEventCallback((uint8 *)command, 1);

        if (!SCPI::PeriodicTask())
        {
            break;
        }
    }

    LOG_ERROR("SCPI tests is failed");

    return false;
}
