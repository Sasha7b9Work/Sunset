// 2025/8/9 09:54:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook.h"
#include "MainPanel/PanelTests/PanelTests.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "Controls/Bitmap.h"
#include "IPPP/Tests/Tests.h"
#include "Controls/StaticBox.h"
#include "Utils/Configurator.h"
#include "System/Events.h"
#include "Controls/Splitter.h"
#include "Controls/Panel.h"


PanelTests *ThePanelTests = nullptr;


PanelTests::PanelTests(Notebook *board, PanelTests *&self) :
    PageNotebook(board, L("Тесты"))
{
    self = this;

    wxPanel::SetName("PanelTests");

    hor_splitter = new Splitter(this, wxSP_3D | wxSP_LIVE_UPDATE);
    hor_splitter->SetMinimumPaneSize(50);  // Минимальный размер панели

    vert_splitter = new Splitter(hor_splitter, wxSP_3D | wxSP_LIVE_UPDATE);
    vert_splitter->SetMinimumPaneSize(50);

    panelLeftTop = new Panel(vert_splitter);

    panelLeftBottom = new Panel(vert_splitter);

    panelRight = new Panel(hor_splitter);

    vert_splitter->SplitHorizontally(panelLeftTop, panelLeftBottom, 200);

    hor_splitter->SplitVertically(vert_splitter, panelRight, 400);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(hor_splitter, 1, wxEXPAND);
    SetSizer(sizer);

    Layout();
}


void PanelTests::CallbackOnEventSize()
{
}
