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


class LibraryCategory
{
public:

    wxString name{ "" };
    std::vector<Test> tests;
};


class Library
{
public:

    bool Read(FileJSON *);

    std::vector<LibraryCategory> categories;

private:

    FileJSON *file;

    bool ParseCategory(LibraryCategory &, pchar name, const rapidjson::Value &);

    bool ParseNameCategory(LibraryCategory &, pchar name_cat);

    bool ParseTest(LibraryCategory &, const rapidjson::Value &);
};
