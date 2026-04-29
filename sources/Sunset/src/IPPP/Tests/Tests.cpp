// 2025/08/19 14:10:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Tests/Tests.h"
#include "Settings/Configurator.h"


void Test::Load(pchar file_name)
{
    Config::SetFile(wxGetCwd() + "/" + file_name);

//    ThePanelConfig->Unpack();

    Config::SetFile("");
}


void Test::Save(pchar file_name)
{
    Config::SetFile(wxGetCwd() + "/" + file_name);

//    ThePanelConfig->Pack();

    Config::SetFile("");
}
