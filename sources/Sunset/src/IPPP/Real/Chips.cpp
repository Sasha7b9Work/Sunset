// 2025/10/17 11:39:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Real/Chips.h"
#include "IPPP/IDevice.h"
#include "Communicator/SPI/SPI.h"


DAC *dacs[DAC::Count];
REG *regs[REG::Count];
FPGA *fpgas[FPGA::Count];


void Chip::Init()
{
    for (int i = 0; i < DAC::Count; i++)
    {
        dacs[i] = new DAC((DAC::E)i);
    }

    for (int i = 0; i < REG::Count; i++)
    {
        regs[i] = new REG((REG::E)i);
    }

    for (int i = 0; i < FPGA::Count; i++)
    {
        fpgas[i] = new FPGA((FPGA::E)i);
    }
}


DAC::DAC(E _v) : v(_v)
{

}


wxString DAC::GetNameDevice() const
{
    return wxString::Format("DAC%d", (int)v);
}


int DAC::BitDepth() const
{
    static const int depth[Count] =
    {
        16,
        16,
        14,
        12,
        16,
        16,
        16
    };

    return depth[v];
}


REG::REG(E _v) : v(_v)
{

}


wxString REG::GetNameDevice() const
{
    return wxString::Format("REG%d", (int)v);
}


int REG::BitDepth() const
{
    static const int depth[Count] =
    {
        24,
        16,
         8,
        24,
        32,
        16,
    };

    return depth[v];
}


FPGA::FPGA(E _v) : v(_v)
{

}


wxString FPGA::GetNameDevice() const
{
    return wxString::Format("FPGA%d", (int)v);
}


int FPGA::BitDepth() const
{
    static const int depth[Count] =
    {
        9,
        8,
        8
    };

    return depth[v];
}


void Chip::WriteValue(uint value) const
{
    if (IsDAC())
    {
        DAC *dac = (DAC *)this;

        int numberDAC = dac->GetNumberDynamicDAC();

        if (numberDAC == 1 || numberDAC == 2)
        {
            SPI::WriteDynamicDAC(dac->GetNumberDynamicDAC(), (uint16)value);

            return;
        }
    }

    IDevice::impl->SendCommand(":%s:WRITE %X", GetNameDevice().c_str().AsChar(), value); //-V111
}


void Chip::WriteWidthToDevice() const
{
    IDevice::impl->SendCommand(":%s:LENGTH %d", GetNameDevice().c_str().AsChar(), BitDepth()); //-V111
}
