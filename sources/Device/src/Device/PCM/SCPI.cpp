// 2025/10/15 14:32:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/PCM/SCPI.h"
#include "Utils/StringUtils.h"
#include "Device/FPGA.h"
#include "Device/Chips.h"
#include "Hardware/HAL/HAL.h"
#include "Device/Device.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdio>

/*
    :PING

    // Просто показать, как работает обработка структур
    :CHIP:REG[0...9]:LENGTH [1...32]

    // Управление регистрами ПЛИС
    :FPGA[0...9]:LENGHT [1...32]
    :FPGA[0...9]:WRITE  [0....(uint)-1]

    // Управление ЦАПами блоков
    :DAC[0...9]:LENGTH [8...32]
    :DAC[0...9]:WRITE [0...(uint)-1]

    // Управление регистрами блоков
    :REG[0...9]:LENGTH [8...32]
    :REG[0...9]:WRITE [0...(uint)-1]
*/


namespace PCM
{
    namespace SCPI
    {
        struct StructParser
        {
            pchar         begin_string;
            bool          (*func)(pchar);
            StructParser *structs;
        };

        static bool Func_Ping(pchar);

        static bool Func_CHIP_REG(pchar);

        static StructParser chip[] =
        {
            { "REG",   Func_CHIP_REG, nullptr },    // :CHIP:REG...
            { nullptr, nullptr,       nullptr }
        };

        static bool Func_FPGA(pchar);
        static bool Func_DAC(pchar);
        static bool Func_REG(pchar);
        static bool Func_SCAN(pchar);

        static StructParser head[] =
        {
            { "PING",  Func_Ping, nullptr },
            { "CHIP",  nullptr,   chip    },
            { "FPGA",  Func_FPGA, nullptr },
            { "DAC",   Func_DAC,  nullptr },
            { "REG",   Func_REG,  nullptr },
            { "SCAN",  Func_SCAN, nullptr },
            { nullptr, nullptr,   nullptr }
        };

        static bool ProcessStructures(pchar, StructParser *);
    }
}


bool PCM::SCPI::Parse(pchar command)
{
    return ProcessStructures(command, head);
}


bool PCM::SCPI::Func_Ping(pchar command)
{
    if (*command != '\0')
    {
        return false;
    }

    SCPI::Send(":PING");

    return true;
}


bool PCM::SCPI::ProcessStructures(pchar command, StructParser *handlers)
{
    if (command[0] == ':')
    {
        command++;
    }

    StructParser *handler = handlers;

    while (handler->begin_string)
    {
        if (SU::BeginWith(command, handler->begin_string))
        {
            if (handler->func)
            {
                pchar new_command = command + std::strlen(handler->begin_string);

                if (*new_command == ':')
                {
                    new_command++;
                }

                return handler->func(new_command);
            }
            else if (handler->structs)
            {
                return ProcessStructures(command + std::strlen(handler->begin_string), handler->structs);
            }
        }

        handler++;
    }

    return true;
}


bool PCM::SCPI::Func_FPGA(pchar command)
{
    if (*command < '0' || *command > '9')
    {
        return false;
    }

    int num_reg = (int)((*command) & 0x0F);

    command++;

    if (*command != ':')
    {
        return false;
    }

    command++;

    if (SU::BeginWith(command, "LENGTH "))                              // :FPGA:REG:LENGTH
    {
        command += std::strlen("LENGTH ");

        char *pos = nullptr;

        uint length = std::strtoul(command, &pos, 16);

        if (SU::CharIs(*pos, " :"))
        {
            FPGA::Reg::SetLength(num_reg, length);

            return true;
        }

        return false;
    }
    else if (SU::BeginWith(command, "WRITE "))                          // :FPGA:REG:WRITE
    {
        command += std::strlen("WRITE ");

        char *pos = nullptr;

        uint value = std::strtoul(command, &pos, 16);

        if (SU::CharIs(*pos, " :"))
        {
//            LOG_WRITE("Write %08X to FPGA%d", value, num_reg);

            FPGA::Reg::Write(num_reg, value);

            return true;
        }

        return false;
    }

    return false;
}


bool PCM::SCPI::Func_SCAN(pchar command)
{
    if (SU::BeginWith(command, "START "))
    {
        command += std::strlen("START ");

        char *pos = nullptr;

        uint period = std::strtoul(command, &pos, 10);

        if (SU::CharIs(*pos, " :"))
        {
            Device::EmergencyStart();

            FPGA::StartScan(period);

            return true;
        }

        return false;
    }
    else if (SU::BeginWith(command, "STOP"))
    {
        Device::EmergencyStop();

        FPGA::StopScan();

        return true;
    }

    return false;
}


bool PCM::SCPI::Func_DAC(pchar command)
{
    if (*command < '0' || *command > '9')
    {
        return false;
    }

    int num_dac = (int)((*command) & 0x0F);

    command++;

    if (*command != ':')
    {
        return false;
    }

    command++;

    if (SU::BeginWith(command, "LENGTH "))                              // :DAC:LENGTH
    {
        command += std::strlen("LENGTH ");

        char *pos = nullptr;

        uint length = std::strtoul(command, &pos, 10);

        if(SU::CharIs(*pos, " :"))
        {
            ChipDAC::Get((ChipDAC::E)num_dac).SetLength(length);

            return true;
        }

        return false;
    }
    else if (SU::BeginWith(command, "WRITE "))                          // :DAC:WRITE
    {
        command += std::strlen("WRITE ");

        char *pos = nullptr;

        uint value = std::strtoul(command, &pos, 16);

        if (SU::CharIs(*pos, " :"))
        {
//            LOG_WRITE("Write %08X to DAC%d", value, num_dac);

            ChipDAC::Get((ChipDAC::E)num_dac).WriteValue(value);

            return true;
        }

        return false;
    }

    return false;
}


bool PCM::SCPI::Func_REG(pchar command)
{
    if (*command < '0' || *command > '9')
    {
        return false;
    }

    int num_reg = (int)((*command) & 0x0F);

    command++;

    if (*command != ':')
    {
        return false;
    }

    command++;

    if (SU::BeginWith(command, "LENGTH "))                              // :REG:LENGTH
    {
        command += std::strlen("LENGTH ");

        char *pos = nullptr;

        uint length = std::strtoul(command, &pos, 16);

        if (SU::CharIs(*pos, " :"))
        {
            ChipREG::Get((ChipREG::E)num_reg).SetLength(length);

            return true;
        }

        return false;
    }
    else if (SU::BeginWith(command, "WRITE "))                          // :REG:WRITE
    {
        command += std::strlen("WRITE ");

        char *pos = nullptr;

        uint value = std::strtoul(command, &pos, 16);

        if (SU::CharIs(*pos, " :"))
        {
//            LOG_WRITE("Write %08X to REG%d", value, num_reg);

            ChipREG::Get((ChipREG::E)num_reg).WriteValue(value);

            return true;
        }

        return false;
    }

    return false;
}


bool PCM::SCPI::Func_CHIP_REG(pchar)
{
    return false;
}


void PCM::SCPI::Send(pchar format, ...)
{
    char message[1024];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    HAL_USART1::TransmitString(message);
}
