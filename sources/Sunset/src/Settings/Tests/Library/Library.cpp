// 2026/05/05 15:11:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Tests/Library/Library.h"


bool Library::Read(FileJSON *_file)
{
    using namespace rapidjson;

    file = _file;

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
                    if (!ParseNameCategory(category, cat_name))
                    {
                        return false;
                    }
                }
                else if (it_value->value.IsObject())
                {
                    if (!ParseTest(category, it_value->value))
                    {
                        return false;
                    }
                }
            }

            categories.push_back(category);
        }
    }

    return true;
}


bool Library::ParseNameCategory(Category &category, pchar name_cat)
{
    category.name = file->GetStringValue(name_cat, "name");

    return category.name[0] != '\0';
}


bool Library::ParseTest(Category &category, const rapidjson::Value &value)
{
    Test test;

    for (auto it = value.MemberBegin(); it != value.MemberEnd(); it++)
    {
        if (it->value.IsString())
        {
            test.name = it->name.GetString();
        }
        else if (it->value.IsObject())
        {

        }
    }

    category.tests.push_back(test);

    return test.name[0] != '\0';
}
