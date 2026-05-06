// 2026/05/05 15:11:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Tests/Library/Library.h"
#include "Settings/FileJSON.h"


void Library::Read(FileJSON *file)
{
    using namespace rapidjson;

    const Document &doc = file->Document();

    if (doc.HasMember("category") && doc["category"].IsArray())
    {
        const rapidjson::Value &cat = doc["category"];          // Категория

        for (SizeType i = 0; i < cat.Size(); i++)
        {
            pchar cat_name = cat[i].GetString();

            categories.push_back({ cat_name });
        }
    }
}
