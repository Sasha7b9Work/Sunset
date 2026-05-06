// 2026/04/29 11:34:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PageTests/PanelCategory.h"
#include "Panels/PageTests/PageTests.h"
#include "Controls/Sizers.h"
#include "Controls/CheckBox.h"
#pragma warning(push, 0)
#include <wx/checkbox.h>
#pragma warning(pop)


PanelCategory *ThePanelCategory = nullptr;


PanelCategory::PanelCategory(wxWindow *parent, PanelCategory *&global) : ScrolledPanel(parent)
{
    global = this;

    SizerVert *main_sizer = new SizerVert();

//    for (int i = 0; i < 30; i++)
//    {
//        CheckBox *check_box = new CheckBox(this, wxString::Format("Category %d", i + 1));
//
//        main_sizer->Add(check_box);
//
//        categories.push_back({ check_box });
//    }
//
//    main_sizer->AddStretchSpacer();

    ScrolledPanel::SetSizer(main_sizer);
}


void PanelCategory::UpdateState()
{
    wxSizer *sizer = GetSizer();

    for (size_t i = 0; i < categories.size(); i++)
    {
        CheckBox *checkbox = categories[i].checkbox;

        sizer->Detach(checkbox);
        checkbox->Destroy();

        sizer->Layout();
        Layout();
    }

    sizer->Clear();

    categories.clear();

    const Library &lib = ThePageTests->GetLibrary();

    for (size_t i = 0; i < lib.categories.size(); i++)
    {
        const ::Category &cat = lib.categories[i];

        CheckBox *checkbox = new CheckBox(this, cat.name);
        sizer->Add(checkbox);
        categories.push_back({ checkbox });
    }

    sizer->AddStretchSpacer();

    sizer->Layout();

    Layout();
}
