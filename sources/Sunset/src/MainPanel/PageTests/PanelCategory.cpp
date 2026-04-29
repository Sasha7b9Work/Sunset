// 2026/04/29 11:34:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainPanel/PageTests/PanelCategory.h"
#include "Controls/Sizers.h"
#include "Controls/CheckBox.h"
#pragma warning(push, 0)
#include <wx/checkbox.h>
#pragma warning(pop)


PanelCategory *ThePanelCategory = nullptr;


PanelCategory::PanelCategory(wxWindow *parent, PanelCategory *&global) : Panel(parent)
{
    global = this;

    SizerVert *main_sizer = new SizerVert();

    for (int i = 0; i < 30; i++)
    {
        CheckBox *check_box = new CheckBox(this, wxString::Format("Category %d", i + 1));
        check_box->SetMinSize({ 500, -1 });

        main_sizer->Add(check_box);
    }

    main_sizer->AddStretchSpacer();

    Panel::SetSizer(main_sizer);

}
