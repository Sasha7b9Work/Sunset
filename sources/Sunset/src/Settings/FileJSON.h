// 2022/09/14 14:35:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <string>
#pragma warning(push, 0)
#include <wx/arrstr.h>
#pragma warning(pop)
#undef Bool
#include <rapidjson/document.h>
#include <vector>


/*
    Файлы конфигурации читают информацию из json-файлов
*/


class FileJSON
{
public:

    virtual ~FileJSON() { Unload(); }

    // Путь относительно каталога с исполняемым файлом
    bool Load(pchar rel_path);

    void Unload();

    pchar GetStringValue(pchar key);

    pchar GetStringValue(pchar key1, pchar key2);

    pchar GetStringValue(pchar key1, pchar key2, pchar key3);

    bool GetVectorStrings(pchar key, wxArrayString &);

    int GetIntValue(pchar key);

    int GetIntValue(pchar key1, pchar key2);

    bool GetBoolValue(pchar key);

    bool IsValid() const { return is_valid; }

    std::vector<float> GetVector3(pchar key, const std::vector<float> &def = {0.0f, 0.0f, 0.0f});

private:

    bool is_valid = false;

    rapidjson::Document document;

    std::string full_path;
};
