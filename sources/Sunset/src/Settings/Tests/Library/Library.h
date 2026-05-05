// 2026/05/05 15:08:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Tests/SettingsTests.h"


// Класс для хранения набора тестов


class FileJSON;


class Category
{
public:

private:

    wxString     name;
    TypeCategory type;
};


class Library
{
public:

    void Read(FileJSON *);

private:

    std::vector<Category> categories;
};
