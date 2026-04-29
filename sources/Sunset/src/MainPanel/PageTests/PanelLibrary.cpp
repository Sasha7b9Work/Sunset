// 2026/04/29 15:56:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainPanel/PageTests/PanelLibrary.h"
#include "Controls/Sizers.h"
#pragma warning(push, 0)
#include <wx/checkbox.h>
#pragma warning(pop)


PanelLibrary *ThePanelLibrary = nullptr;


PanelLibrary::PanelLibrary(wxWindow *parent, PanelLibrary *&global) : Panel(parent)
{
    global = this;

    SizerVert *main_sizer = new SizerVert();

    for (int i = 0; i < 10; i++)
    {
//        main_sizer->Add(new wxCheckBox(this, wxID_ANY, wxString::Format("")));
    }

    Panel::SetSizer(main_sizer);
}
