// 2026/05/05 15:08:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Класс для хранения набора тестов


class FileJSON;


class _Category
{
public:

private:

    wxString name;
};


class Library
{
public:

    void Read(FileJSON *);

private:

    std::vector<_Category> categories;
};
