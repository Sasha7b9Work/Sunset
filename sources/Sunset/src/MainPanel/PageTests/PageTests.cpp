// 2025/8/9 09:54:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Notebook.h"
#include "MainPanel/PageTests/PageTests.h"
#include "MainPanel/PageTests/PanelCategory.h"
#include "MainPanel/PageTests/PanelLibrary.h"
#include "MainPanel/PageTests/PanelViewTest.h"
#include "MainWindow.h"
#include "Utils/SystemDepend.h"
#include "Controls/Bitmap.h"
#include "IPPP/Tests/Tests.h"
#include "Controls/StaticBox.h"
#include "Utils/Configurator.h"
#include "System/Events.h"
#include "Controls/Splitter.h"
#include "Controls/Panel.h"


PageTests *ThePageTests = nullptr;


PageTests::PageTests(Notebook *board, PageTests *&global) :
    PageNotebook(board, L("Тесты"))
{
    global = this;

    wxPanel::SetName("PageTests");

    hor_splitter = new Splitter(this, wxSP_3D | wxSP_LIVE_UPDATE);
    hor_splitter->SetMinimumPaneSize(50);  // Минимальный размер панели

    vert_splitter = new Splitter(hor_splitter, wxSP_3D | wxSP_LIVE_UPDATE);
    vert_splitter->SetMinimumPaneSize(50);

    panel_category = new PanelCategory(vert_splitter, ThePanelCategory);

    panel_library = new PanelLibrary(vert_splitter, ThePanelLibrary);

    panel_view_test = new PanelViewTest(hor_splitter, ThePanelViewTest);

    vert_splitter->SplitHorizontally(panel_category, panel_library, 200);

    hor_splitter->SplitVertically(vert_splitter, panel_view_test, 400);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(hor_splitter, 1, wxEXPAND);
    SetSizer(sizer);

    Layout();
}


void PageTests::CallbackOnEventSize()
{
}
