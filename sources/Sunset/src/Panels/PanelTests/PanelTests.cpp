// 2025/8/9 09:54:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/Panels.h"
#include "Panels/PanelTests/PanelTests.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "Controls/Bitmap.h"
#include "IPPP/Tests/Tests.h"
#include "Controls/StaticBox.h"
#include "Utils/Configurator.h"
#include "System/Events.h"


PanelTests *ThePanelTests = nullptr;


PanelTests::PanelTests(PanelBoard *board, PanelTests *&self) : Panel(board, L("Тесты"))
{
    self = this;

    wxPanel::SetName("PanelTests");
}


void PanelTests::CallbackOnEventSize()
{

}
