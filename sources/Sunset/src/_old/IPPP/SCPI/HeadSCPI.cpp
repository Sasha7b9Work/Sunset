// 2025/10/23 12:29:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/SCPI/SCPI.h"
#include "Utils/StringUtils.h"
#include "IPPP/Real/PinsDevice.h"


namespace SCPI
{
    static bool FuncPinIn(pchar);
    static bool FuncLog(pchar);
    static bool FuncWarning(pchar);
    static bool FuncError(pchar);
    static bool FuncLogMessage(pchar type, pchar message);
    static bool FuncPing(pchar);

    StructSCPI head[] =
    {
        { "PININ",    FuncPinIn,   nullptr },
        { "LOG",      FuncLog,     nullptr },
        { "WARNING",  FuncWarning, nullptr },
        { "ERROR",    FuncError,   nullptr },
        { "PING",     FuncPing,    nullptr },
        { nullptr,    nullptr,     nullptr }
    };
}


bool SCPI::FuncPinIn(pchar command)
{
    Words words{ command };

    SU::SplitToWords(command, words, " :");

    if (words.Size() == 3 && words[1] == "STATE")
    {
        for (int i = 0; ; i++)
        {
            PinInDevice *pin = pinsD[i];

            if (pin == nullptr)
            {
                return false;
            }

            if (pin->Name() == words[0])
            {
                uint value = SU::UIntFromString(words[2].c_str());

                pin->SetState(value != 0);

                return true;
            }
        }

        return false;
    }

    return false;
}


bool SCPI::FuncLog(pchar command)
{
    return FuncLogMessage("LOG    ", command);
}


bool SCPI::FuncWarning(pchar command)
{
    return FuncLogMessage("WARNING", command);
}


bool SCPI::FuncError(pchar command)
{
    return FuncLogMessage("ERROR  ", command);
}


bool SCPI::FuncLogMessage(pchar type, pchar message)
{
    Log::LogMCU(type, message);

    return true;
}


bool SCPI::FuncPing(pchar)
{
    LOG_WRITE("Ping");

    return true;
}
