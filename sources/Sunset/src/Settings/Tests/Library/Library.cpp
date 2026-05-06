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
        if (it->value.IsObject())
        {
            pchar cat_name = it->name.GetString();
            const auto &cat_value = it->value;

            Category category;

            for (auto it_value = cat_value.MemberBegin(); it_value != cat_value.MemberEnd(); it_value++)
            {
                if (it_value->value.IsString())
                {
                    wxString name = file->GetStringValue(cat_name, "name");
                    if (name[0])
                    {
                        category.name = name;
                    }
                    else
                    {
                        return false;
                    }
                }
                else if (it_value->value.IsObject())
                {
                    int i = 0;
                }
            }

            categories.push_back(category);
        }
    }

    return true;
}


void Category::Clear()
{
    *this = Category();
}
