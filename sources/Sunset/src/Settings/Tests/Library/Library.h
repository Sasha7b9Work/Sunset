// 2026/05/05 15:08:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Tests/SettingsTests.h"


// Класс для хранения набора тестов


class FileJSON;
class PanelCategory;


class Category
{
public:

    wxString     name;
};


class Library
{
public:

    void Read(FileJSON *);

    std::vector<Category> categories;
};
