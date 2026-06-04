// 2026/05/05 15:08:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Tests/SettingsTests.h"
#include "Settings/FileJSON.h"


// Класс для хранения набора тестов


class FileJSON;
class PanelCategory;


class Test
{
public:

    wxString _name{ "" };
};


class Category
{
public:

    wxString name{ "" };
    std::vector<Test> tests;
};


class Library
{
public:

    bool Read(FileJSON *);

    std::vector<Category> categories;

private:

    FileJSON *file;

    bool ParseCategory(Category &, pchar name, const rapidjson::Value &);

    bool ParseNameCategory(Category &, pchar name_cat);

    bool ParseTest(Category &, const rapidjson::Value &);
};
