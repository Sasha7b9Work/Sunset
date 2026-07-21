// 2026/04/29 15:55:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Panel.h"


class LibraryCategory;
class wxListView;
class wxImageList;


class PanelLibrary : public Panel
{
public:

    PanelLibrary(wxWindow *, PanelLibrary *&);
    ~PanelLibrary();

    void BuildListTests(std::vector<const LibraryCategory *> &categories);

    void ClearList();                                   // Очистить весь список
    void AddItem(const wxString &);                     // Добавить элемент без иконки
    void AddItem(const wxString &, int iconIndex);      // Добавить элемент с иконкой
    int AddIcon(const wxString &iconPath);              // Загрузить иконку в imageList
    void SetItemIcon(long itemIndex, int iconIndex);    // Установить иконку существующему элементу

private:

    wxListView *listView = nullptr;
    wxImageList *imageList = nullptr;
};
