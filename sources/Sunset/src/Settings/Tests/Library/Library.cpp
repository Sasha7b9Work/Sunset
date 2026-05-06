// 2026/05/05 15:11:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Tests/Library/Library.h"
#include "Settings/FileJSON.h"


bool Library::Read(FileJSON *file)
{
    using namespace rapidjson;

    const Document &doc = file->Document();

    for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); it++)
    {
        pchar cat_name = it->name.GetString();

        if (it->value.IsObject())
        {
            wxString name = file->GetStringValue(cat_name, "name");

            if (name[0])
            {
                categories.push_back({ name });
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}
