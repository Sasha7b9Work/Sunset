// 2026/04/29 11:33:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Panel.h"


class CheckBox;
class Library;
class LibraryCategory;


class PanelCategory : public ScrolledPanel
{
public:

    PanelCategory(wxWindow *, PanelCategory *&);

    // Обновить состояние в соответствии с библиотеками
    void UpdateState(const Library &);

private:

    struct Category
    {
        CheckBox *checkbox;             // Чекбокс, которым выбирается данная категория
        const LibraryCategory &category;
    };

    std::vector<Category> categories;

    void OnEventCheckBox(wxCommandEvent &);

    // Построить список тестов для категорий выбранных чекбоксами
    void BuildListTests();
};
