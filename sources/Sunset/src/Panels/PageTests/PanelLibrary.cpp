// 2026/04/29 15:56:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PageTests/PanelLibrary.h"
#include "Controls/Sizers.h"
#include "Settings/Tests/Library/Library.h"
#pragma warning(push, 0)
#include <wx/checkbox.h>
#include <wx/listctrl.h>
#pragma warning(pop)


PanelLibrary *ThePanelLibrary = nullptr;


PanelLibrary::PanelLibrary(wxWindow *parent, PanelLibrary *&global) : Panel(parent, wxSIMPLE_BORDER)
{
    global = this;

    SizerVert *main_sizer = new SizerVert();

    listView = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_LIST | wxLC_SINGLE_SEL);

    imageList = new wxImageList(16, 16, true, 0);

    listView->SetImageList(imageList, wxIMAGE_LIST_SMALL);

    main_sizer->Add(listView, 1, wxEXPAND | wxALL, 0);

    Panel::SetSizer(main_sizer);

    for (int i = 0; i < 10; i++)
    {
        AddItem(wxString::Format("Test %d", i));
    }
}


PanelLibrary::~PanelLibrary()
{
    if (imageList)
    {
        delete imageList;
    }
}


void PanelLibrary::ClearList()
{
    if (listView)
    {
        listView->DeleteAllItems();
    }
}


void PanelLibrary::AddItem(const wxString &text, int iconIndex)
{
    if (!listView) return;

    listView->InsertItem(listView->GetItemCount(), text, iconIndex);
}


void PanelLibrary::AddItem(const wxString &text)
{
    AddItem(text, -1);
}


int PanelLibrary::AddIcon(const wxString &iconPath)
{
    if (!imageList) return -1;

    wxBitmap bitmap(iconPath, wxBITMAP_TYPE_PNG);

    if (bitmap.IsOk())
    {
        return imageList->Add(bitmap);
    }
    return -1;
}


void PanelLibrary::SetItemIcon(long itemIndex, int iconIndex)
{
    if (listView && itemIndex >= 0 && itemIndex < listView->GetItemCount())
    {
        listView->SetItemImage(itemIndex, iconIndex);
    }
}


void PanelLibrary::BuildListTests(std::vector<const ::Category *> &categories)
{
    ClearList();

    for (auto cat : categories)
    {
        for (auto test : cat->tests)
        {
            AddItem(test.name);
        }
    }
}
