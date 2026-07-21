// 2025/10/17 11:39:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Эта структура описывает регистр ПЛИС, регистр или ЦАП блока
struct Chip
{
    virtual ~Chip() { }

    static void Init();

    // Под этим именем она обозначена в плате контроллера
    virtual wxString GetNameDevice() const = 0;

    // Ширина в битах
    virtual int BitDepth() const = 0;

    // Послать значение в контроллер
    void WriteValue(uint) const;

    // Послать в контроллер размер регистра
    void WriteWidthToDevice() const;

    virtual bool IsDAC() const { return false; }
};


struct DAC : public Chip
{
    enum E
    {
        _0_ChannelC_Form,   // Формирователь
        _1_ChannelC_Meas,   // Измеритель
        _2_ChannelB_DAC1,   // База DAC1
        _3_ChannelB_DAC2,   // База DAC2
        _4_ChannelS_DAC1,
        _5_ChannelS_DAC2,
        _6_Source_50V,
        Count
    };

    DAC(E i);

    virtual wxString GetNameDevice() const override;

    virtual int BitDepth() const override;

    virtual bool IsDAC() const override
    {
        return true;
    }

    int GetNumberDynamicDAC() const
    {
        if (v == _0_ChannelC_Form)
        {
            return 1;
        }
        else if (v == _5_ChannelS_DAC2)
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }

private:

    E v = Count;
};


struct REG : public Chip
{
    enum E
    {
        _0_Source3kV,
        _1_Commutator,
        _2_ChannelC,
        _3_ChannelB,
        _4_ChannelS,
        _5_MeasCurrent,
        Count
    };

    REG(E i);

    virtual wxString GetNameDevice() const override;

    virtual int BitDepth() const override;

private:

    E v = Count;
};


struct FPGA : public Chip
{
    enum E
    {
        _0,
        _1,
        _2,
        Count
    };

    FPGA(E i);

    virtual wxString GetNameDevice() const override;

    virtual int BitDepth() const override;

private:

    E v = Count;
};


extern DAC *dacs[DAC::Count];
extern REG *regs[REG::Count];
extern FPGA *fpgas[FPGA::Count];
