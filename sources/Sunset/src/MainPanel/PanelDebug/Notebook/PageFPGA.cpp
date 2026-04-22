// 2025/10/13 13:54:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainPanel/PanelDebug/Notebook/PageFPGA.h"
#include "MainPanel/PanelDebug/Notebook/Register.h"
#include "Utils/String.h"
#include "Utils/SystemDepend.h"
#include "IPPP/IDevice.h"


PageFPGA *PageFPGA::self = nullptr;


PageFPGA::PageFPGA(wxNotebook *parent) :
    PageChip(parent, "ПЛИС")
{
    self = this;

    {
        Register *fpga0 = new RegFPGA(this, fpgas[0]);

        std::vector<StructDescription> desc0; //-V827

        std::vector<StructDescription::CommandStruct> commandsScan; //-V827 //-V826
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0000, L("Выключена")});
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0001, "SIN+" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0010, "SIN-" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0011, "AC" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0100, "DC-" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0101, "DC+" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0110, "IMP+" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b0111, "IMP-" });
        commandsScan.emplace_back(StructDescription::CommandStruct{ 0b1000, "IMP-CVC" });
        desc0.emplace_back(StructDescription{ 0, 4, L("разв"), L("развёртка"), { true, commandsScan } });

        std::vector<StructDescription::CommandStruct> commandsTypeElem; //-V826
        commandsTypeElem.emplace_back(StructDescription::CommandStruct{ 0, L("Диод") });
        commandsTypeElem.emplace_back(StructDescription::CommandStruct{ 1, L("Транзистор") });
        desc0.emplace_back(StructDescription{ 4, 1, "з", L("запуск"), { true, commandsTypeElem } });

        std::vector<StructDescription::CommandStruct> commandsCurvers; //-V827 //-V826
        for (int i = 0; i < 16; i++)
        {
            commandsCurvers.emplace_back(StructDescription::CommandStruct{ (uint16)i, String("%d", i).c_str() });
        }
        desc0.emplace_back(StructDescription{ 5, 4, L("к"), L("количество кривых ВАХ для DC+, DC-"), { true, commandsCurvers } });

        fpga0->SetDescriptionBits(0, desc0);

        AppendRegister(fpga0);
    }

    {
        Register *fpga1 = new RegFPGA(this, fpgas[1]);

        std::vector<StructDescription> desc1;

        std::vector<StructDescription::CommandStruct> commandsDuration; //-V827 //-V826
        desc1.emplace_back(StructDescription{ 0, fpga1->GetChip()->BitDepth(), "Ти = N * Sk", "", {true, commandsDuration}});

        fpga1->SetDescriptionBits(0, desc1);

        AppendRegister(fpga1);
    }

    {
        Register *fpga2 = new RegFPGA(this, fpgas[2]);

        std::vector<StructDescription> desc2;

        std::vector<StructDescription::CommandStruct> commandsDuration; //-V827 //-V826
        desc2.emplace_back(StructDescription{ 0, fpga2->GetChip()->BitDepth(), "RunDelay = N * 1us", "", { true, commandsDuration } });

        fpga2->SetDescriptionBits(0, desc2);

        AppendRegister(fpga2);
    }
}
