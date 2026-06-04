// 2026/05/05 15:11:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Tests/Library/Library.h"


bool Library::Read(FileJSON *_file)
{
    using namespace rapidjson;

    file = _file;

    const Document &doc = file->Document();

    for (auto it = doc.MemberBegin(); it != doc.MemberEnd(); ++it)
    {
        if (it->value.IsObject())
        {
            Category category;

            if (ParseCategory(category, it->name.GetString(), it->value))
            {
                categories.push_back(category);
            }
            else
            {
                LOG_ERROR("Load library from %s is FAIL!", _file->GetFullPath().c_str());

                return false;
            }
        }
    }

    LOG_WRITE("Load library from %s is OK!", _file->GetFullPath().c_str());

    return true;
}


bool Library::ParseCategory(Category &category, pchar name, const rapidjson::Value &value)
{
    for (auto it_value = value.MemberBegin(); it_value != value.MemberEnd(); ++it_value)
    {
        if (it_value->value.IsString())
        {
            if (!ParseNameCategory(category, name))
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

    for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it)
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
