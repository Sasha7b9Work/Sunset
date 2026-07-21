// 2025/10/07 12:05:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelDebug/Notebook/PageChannelB.h"
#include "Panels/PanelDebug/Notebook/Register.h"


PageChannelB *PageChannelB::self = nullptr;


PageChannelB::PageChannelB(wxNotebook *parent) :
    PageChip(parent, "Канал B")
{
    self = this;

    {
        RegAD5531 *dac2 = new RegAD5531(this, dacs[DAC::_2_ChannelB_DAC1], "");

        std::vector<StructDescription> desc2;
        desc2.emplace_back(StructDescription{ 0, dac2->GetChip()->BitDepth(), "", "", { true } });
        dac2->SetDescriptionBits(0, desc2);

        AppendRegister(dac2);
    }

    {
        RegAD5531 *dac3 = new RegAD5531(this, dacs[DAC::_3_ChannelB_DAC2], "");

        std::vector<StructDescription> desc3;
        desc3.emplace_back(StructDescription{ 0, dac3->GetChip()->BitDepth(), "", "", { true } });
        dac3->SetDescriptionBits(0, desc3);

        AppendRegister(dac3);
    }

    {
        RegFPGA *reg3 = new RegFPGA(this, regs[REG::_3_ChannelB]);

        std::vector<StructDescription> desc3; //-V827

        std::vector<StructDescription::CommandStruct> commandsLimitCurrent; //-V827 //-V826
        commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b0000, "20 нA" });
        commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b0001, "200 нA" });
        commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b0010, "2 мкA" });
        commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b0011, "20 мкA" });
        commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b0100, "200 мкA" });
        commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b0101, "2 мA" });
        commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b0110, "20 мA" });
        commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b0111, "200 мA" });
        commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b1000, "2 A" });
        commandsLimitCurrent.emplace_back(StructDescription::CommandStruct{ 0b1001, "10 A" });
        desc3.emplace_back(StructDescription{ 0, 4, "ппт", "Предел по току", { false, commandsLimitCurrent } });

        std::vector<StructDescription::CommandStruct> commandsOS_OPA; //-V826
        commandsOS_OPA.emplace_back(StructDescription::CommandStruct{ 0, "В режиме U" });
        commandsOS_OPA.emplace_back(StructDescription::CommandStruct{ 1, "В режиме I" });
        desc3.emplace_back(StructDescription{ 4, 1, "о", "Ucl Подключение ОС вых ОУ", { false, commandsOS_OPA } });

        std::vector<StructDescription::CommandStruct> commandsZeroCorr; //-V826
        commandsZeroCorr.emplace_back(StructDescription::CommandStruct{ 0, "Выключен" });
        commandsZeroCorr.emplace_back(StructDescription::CommandStruct{ 1, "Включен" });
        desc3.emplace_back(StructDescription{ 5, 1, "г", "CLBR Геркон \"Корр. 0 измерителя Uии\"", { false, commandsZeroCorr } });

        std::vector<StructDescription::CommandStruct> commandsAz; //-V826
        commandsAz.emplace_back(StructDescription::CommandStruct{ 0, "Стоп" });
        commandsAz.emplace_back(StructDescription::CommandStruct{ 1, "Измерение" });
        desc3.emplace_back(StructDescription{ 6, 1, "р", "Az Режим", { false, commandsAz } });

        std::vector<StructDescription::CommandStruct> commandsCopositor; //-V826
        commandsCopositor.emplace_back(StructDescription::CommandStruct{ 0b0, "Включение сглаж. Ёмкости (в диапазонах 200нА, 2мкА)" });
        commandsCopositor.emplace_back(StructDescription::CommandStruct{ 0b1, "Выкл. сглаживающей ёмкости (в остальных диапазонах)" });
        desc3.emplace_back(StructDescription{ 7, 1, "ё", "Сглаживающая ёмкость", { false, commandsCopositor } });

        std::vector<StructDescription::CommandStruct> commandsDivider;
        commandsDivider.emplace_back(StructDescription::CommandStruct{ 0x00, "Делитель 1:1  в режиме \"источник U\"." });
        commandsDivider.emplace_back(StructDescription::CommandStruct{ 0x01, "Делитель 1:4 в режиме \"источник U\"." });
        commandsDivider.emplace_back(StructDescription::CommandStruct{ 0x10, "Делитель 1:10 в режиме \"источник U\"." });
        desc3.emplace_back(StructDescription{ 8, 2, "д", "Делитель", {false, commandsDivider} });

        std::vector<StructDescription::CommandStruct> commandsQ10; //-V826
        commandsQ10.emplace_back(StructDescription::CommandStruct{ 0, "Источник U - ограничение по I" });
        commandsQ10.emplace_back(StructDescription::CommandStruct{ 1, "Режим 1:1 источника тока" });
        desc3.emplace_back(StructDescription{ 10, 1, "р", "Ui Режим", { false, commandsQ10 } });

        std::vector<StructDescription::CommandStruct> commandsRange; //-V826
        commandsRange.emplace_back(StructDescription::CommandStruct{ 0b00, "1" });
        commandsRange.emplace_back(StructDescription::CommandStruct{ 0b01, "4" });
        commandsRange.emplace_back(StructDescription::CommandStruct{ 0b10, "10" });
        desc3.emplace_back(StructDescription{ 12, 2, "д", "Диапазон измерения", { false, commandsRange } });

        std::vector<StructDescription::CommandStruct> commandsCap; //-V826
        commandsCap.emplace_back(StructDescription::CommandStruct{ 0, "Включён" });
        commandsCap.emplace_back(StructDescription::CommandStruct{ 1, "Отключён" });
        desc3.emplace_back(StructDescription{ 14, 1, "к", "On14 Дополнительный конденсатор DET", { false, commandsCap } });

        std::vector<StructDescription::CommandStruct> commandsBuffer; //-V826
        commandsBuffer.emplace_back(StructDescription::CommandStruct{ 0, "2" });
        commandsBuffer.emplace_back(StructDescription::CommandStruct{ 1, "3" });
        desc3.emplace_back(StructDescription{ 15, 1, "б", "On15 Выбор буфера", { false, commandsBuffer } });

        std::vector<StructDescription::CommandStruct> commandsMeas; //-V826
        commandsMeas.emplace_back(StructDescription::CommandStruct{ 0, "I" });
        commandsMeas.emplace_back(StructDescription::CommandStruct{ 1, "U" });
        desc3.emplace_back(StructDescription{ 17, 1, "и", "Режим измерения", { false, commandsMeas } });

        std::vector<StructDescription::CommandStruct> commandsImp; //-V826
        commandsImp.emplace_back(StructDescription::CommandStruct{ 0, "Запрещены" });
        commandsImp.emplace_back(StructDescription::CommandStruct{ 1, "Разрешены" });
        desc3.emplace_back(StructDescription{ 18, 1, "и", "ImpV Импульсы в канале B режим высокого напряжения", { false, commandsImp } });

        std::vector<StructDescription::CommandStruct> commandsImpI; //-V826
        commandsImpI.emplace_back(StructDescription::CommandStruct{ 0, "Запрещены" });
        commandsImpI.emplace_back(StructDescription::CommandStruct{ 1, "Разрешены" });
        desc3.emplace_back(StructDescription{ 19, 1, "и", "ImpI Импульсы в режиме высокого тока", { false, commandsImpI } });

        std::vector<StructDescription::CommandStruct> commandsSignal;
        commandsSignal.emplace_back(StructDescription::CommandStruct{ 0, "" });
        commandsSignal.emplace_back(StructDescription::CommandStruct{ 1, "" });
        desc3.emplace_back(StructDescription{ 20, 1, "c", "(Onl1) – 1RUout Сигнал включения 2А диапазона", {false, commandsSignal} });

        std::vector<StructDescription::CommandStruct> commands55; //-V826
        commands55.emplace_back(StructDescription::CommandStruct{ 0, "Отключено" });
        commands55.emplace_back(StructDescription::CommandStruct{ 1, "Включено" });
        desc3.emplace_back(StructDescription{ 22, 1, "5", "U+ 55 В", { false, commands55 } });

        reg3->SetDescriptionBits(0, desc3);

        {
            {
                std::vector<ModeDescripion> modesU;

                ModeDescripion modeU_40V{ "40В", "40В 20В 2В/ст 1:1 40В" };
                modeU_40V.state.emplace_back(StateBit{ 8, false });
                modeU_40V.state.emplace_back(StateBit{ 9, false });
                modeU_40V.state.emplace_back(StateBit{ 10, false });
                modeU_40V.state.emplace_back(StateBit{ 12, false });
                modeU_40V.state.emplace_back(StateBit{ 13, false });
                modeU_40V.state.emplace_back(StateBit{ 17, true });
                modesU.emplace_back(modeU_40V);

                ModeDescripion modeU_10V{ "10В", "10В 5В 0.5В/ст 1:4 10В" };
                modeU_10V.state.emplace_back(StateBit{ 8, true });
                modeU_10V.state.emplace_back(StateBit{ 9, false });
                modeU_10V.state.emplace_back(StateBit{ 10, false });
                modeU_10V.state.emplace_back(StateBit{ 12, true });
                modeU_10V.state.emplace_back(StateBit{ 13, false });
                modeU_10V.state.emplace_back(StateBit{ 17, true });
                modesU.emplace_back(modeU_10V);

                ModeDescripion modeU_4V{ "4В", "4В 2В 0.2В/ст 1:10 4В" };
                modeU_4V.state.emplace_back(StateBit{ 8, false });
                modeU_4V.state.emplace_back(StateBit{ 9, true });
                modeU_4V.state.emplace_back(StateBit{ 10, false });
                modeU_4V.state.emplace_back(StateBit{ 12, false });
                modeU_4V.state.emplace_back(StateBit{ 13, true });
                modeU_4V.state.emplace_back(StateBit{ 17, true });
                modesU.emplace_back(modeU_4V);

                reg3->AppendModes(L("Источник напряжения"), modesU);
            }

            {
                std::vector<ModeDescripion> modesI;

                ModeDescripion modeI_10A{ "10A", "10A 5A 0.5A/ст 0.2 Ом 20A" };
                modeI_10A.state.emplace_back(StateBit{ 0, true });
                modeI_10A.state.emplace_back(StateBit{ 1, false });
                modeI_10A.state.emplace_back(StateBit{ 2, false });
                modeI_10A.state.emplace_back(StateBit{ 3, true });
                modeI_10A.state.emplace_back(StateBit{ 10, true });
                modeI_10A.state.emplace_back(StateBit{ 17, false });
                modesI.emplace_back(modeI_10A);

                ModeDescripion modeI_2A{ "2A", "2A 1A 0.1A/ст 1 Ом 2A" };
                modeI_2A.state.emplace_back(StateBit{ 0, false });
                modeI_2A.state.emplace_back(StateBit{ 1, false });
                modeI_2A.state.emplace_back(StateBit{ 2, false });
                modeI_2A.state.emplace_back(StateBit{ 3, true });
                modeI_2A.state.emplace_back(StateBit{ 10, true });
                modeI_2A.state.emplace_back(StateBit{ 17, false });
                modesI.emplace_back(modeI_2A);

                ModeDescripion modeI_200mA{ "200mA", "200mA 100mA 10mA/ст 10 Ом 200mA" };
                modeI_200mA.state.emplace_back(StateBit{ 0, true });
                modeI_200mA.state.emplace_back(StateBit{ 1, true });
                modeI_200mA.state.emplace_back(StateBit{ 2, true });
                modeI_200mA.state.emplace_back(StateBit{ 3, false });
                modeI_200mA.state.emplace_back(StateBit{ 10, true });
                modeI_200mA.state.emplace_back(StateBit{ 17, false });
                modesI.emplace_back(modeI_200mA);

                ModeDescripion modeI_20mA{ "20mA", "20mA 10mA 1mA/ст 100 Ом 20mA" };
                modeI_20mA.state.emplace_back(StateBit{ 0, false });
                modeI_20mA.state.emplace_back(StateBit{ 1, true });
                modeI_20mA.state.emplace_back(StateBit{ 2, true });
                modeI_20mA.state.emplace_back(StateBit{ 3, false });
                modeI_20mA.state.emplace_back(StateBit{ 10, true });
                modeI_20mA.state.emplace_back(StateBit{ 17, false });
                modesI.emplace_back(modeI_20mA);

                ModeDescripion modeI_2mA{ "2mA", "2mA 1mA 0.1mA/ст 1 kОм 2mA" };
                modeI_2mA.state.emplace_back(StateBit{ 0, true });
                modeI_2mA.state.emplace_back(StateBit{ 1, false });
                modeI_2mA.state.emplace_back(StateBit{ 2, true });
                modeI_2mA.state.emplace_back(StateBit{ 3, false });
                modeI_2mA.state.emplace_back(StateBit{ 10, true });
                modeI_2mA.state.emplace_back(StateBit{ 17, false });
                modesI.emplace_back(modeI_2mA);

                ModeDescripion modeI_200uA{ "200uA", "200uA 100uA 10uA/ст 10 kОм 200uA" };
                modeI_200uA.state.emplace_back(StateBit{ 0, false });
                modeI_200uA.state.emplace_back(StateBit{ 1, false });
                modeI_200uA.state.emplace_back(StateBit{ 2, true });
                modeI_200uA.state.emplace_back(StateBit{ 3, false });
                modeI_200uA.state.emplace_back(StateBit{ 10, true });
                modeI_200uA.state.emplace_back(StateBit{ 17, false });
                modesI.emplace_back(modeI_200uA);

                ModeDescripion modeI_20uA{ "20uA", "20uA 10uA 1uA/ст 100 kОм 20uA" };
                modeI_20uA.state.emplace_back(StateBit{ 0, true });
                modeI_20uA.state.emplace_back(StateBit{ 1, true });
                modeI_20uA.state.emplace_back(StateBit{ 2, false });
                modeI_20uA.state.emplace_back(StateBit{ 3, false });
                modeI_20uA.state.emplace_back(StateBit{ 10, true });
                modeI_20uA.state.emplace_back(StateBit{ 17, false });
                modesI.emplace_back(modeI_20uA);

                ModeDescripion modeI_2uA{ "2uA", "2uA 1uA 0.1uA/ст 1 MОм 2uA" };
                modeI_2uA.state.emplace_back(StateBit{ 0, false });
                modeI_2uA.state.emplace_back(StateBit{ 1, true });
                modeI_2uA.state.emplace_back(StateBit{ 2, false });
                modeI_2uA.state.emplace_back(StateBit{ 3, false });
                modeI_2uA.state.emplace_back(StateBit{ 10, true });
                modeI_2uA.state.emplace_back(StateBit{ 17, false });
                modesI.emplace_back(modeI_2uA);

                ModeDescripion modeI_200nA{ "200nA", "200nA 100nA 10nA/ст 10 MОм 200nA" };
                modeI_200nA.state.emplace_back(StateBit{ 0, true });
                modeI_200nA.state.emplace_back(StateBit{ 1, false });
                modeI_200nA.state.emplace_back(StateBit{ 2, false });
                modeI_200nA.state.emplace_back(StateBit{ 3, false });
                modeI_200nA.state.emplace_back(StateBit{ 10, true });
                modeI_200nA.state.emplace_back(StateBit{ 17, false });
                modesI.emplace_back(modeI_200nA);

                ModeDescripion modeI_20nA{ "20nA", "20nA 10nA 1nA/ст 100 MОм 20nA" };
                modeI_20nA.state.emplace_back(StateBit{ 0, false });
                modeI_20nA.state.emplace_back(StateBit{ 1, false });
                modeI_20nA.state.emplace_back(StateBit{ 2, false });
                modeI_20nA.state.emplace_back(StateBit{ 3, false });
                modeI_20nA.state.emplace_back(StateBit{ 10, true });
                modeI_20nA.state.emplace_back(StateBit{ 17, false });
                modesI.emplace_back(modeI_20nA);

                reg3->AppendModes(L("Источник тока"), modesI);
            }
        }

        AppendRegister(reg3);
    }
}
