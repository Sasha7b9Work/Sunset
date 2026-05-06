// 2022/09/14 14:34:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/FileJSON.h"
#include "Utils/StringUtils.h"
#pragma warning(push, 0)
#include <wx/file.h>
#pragma warning(pop)


#define CHECK_ON_VALID_STRING   if (!is_valid) { LOG_ERROR("Configuration file is not valid"); return nullptr; }
#define CHECK_ON_VALID_INT      if (!is_valid) { LOG_ERROR("Configuration file is not valid"); return -1; }
#define CHECK_ON_VALID_BOOL     if (!is_valid) { LOG_ERROR("Configuration file is not valid"); return false; }


bool FileJSON::Load(pchar rel_path)
{
    Unload();

    full_path = wxGetCwd() + "/" + rel_path;

    wxFile file;
    if (!file.Open(full_path))
    {
        LOG_ERROR("Can't load configuration file \"%s\"", full_path.c_str());
        return false;
    }

    wxFileOffset fileSize = file.Length();
    if (fileSize == wxInvalidOffset)
    {
        LOG_ERROR("Can't get size of file \"%s\"", full_path.c_str());
        return false;
    }

    // Выделяем буфер с учётом размера файла +1 для нуль-терминатора
    char *buffer = new char[(size_t)fileSize + 1];

    // Читаем файл
    ssize_t bytesRead = file.Read(buffer, (size_t)fileSize);
    if (bytesRead != fileSize)
    {
        LOG_ERROR("Can't read file \"%s\" completely", full_path.c_str());
        delete[] buffer;
        return false;
    }

    buffer[fileSize] = '\0';

    // Используем rapidjson
    document.Parse(buffer);

    delete[] buffer;  // освобождаем буфер сразу после парсинга

    if (document.HasParseError())
    {
        LOG_ERROR("Can't parse configuration file \"%s\". Error %d, offset %u",
            full_path.c_str(), document.GetParseError(), document.GetErrorOffset());

        return false;
    }

    is_valid = true;

    return true;
}


void FileJSON::Unload()
{
    is_valid = false;
}


int FileJSON::GetIntValue(pchar key)
{
    CHECK_ON_VALID_INT;

    rapidjson::Value::ConstMemberIterator it = document.FindMember(key);

    if (it != document.MemberEnd() && it->value.IsInt())
    {
        return it->value.GetInt();
    }

    LOG_ERROR("Can't find value for \"%s\"", key);

    return -1;
}


std::vector<float> FileJSON::GetVector3(pchar key, const std::vector<float> &def)
{
    pchar str_value = GetStringValue(key);

    Words words("");

    SU::SplitToWords(str_value, words, ", ");

    if (words.GetArrayElementCount() != 3)
    {
        LOG_ERROR("Bad vector for key \"%s\"", key);

        return def;
    }

    std::vector<float> result;

    for (int i = 0; i < 3; i++)
    {
        result.push_back((float)SU::ASCIItoFloat(words[i].c_str()));
    }

    return result;
}


bool FileJSON::GetBoolValue(pchar key)
{
    CHECK_ON_VALID_BOOL;

    rapidjson::Value::ConstMemberIterator it = document.FindMember(key);

    if (it != document.MemberEnd() && it->value.IsInt())
    {
        return it->value.GetInt() != 0;
    }

    LOG_ERROR("Can't find value for \"%s\"", key);

    return false;
}


int FileJSON::GetIntValue(pchar key1, pchar key2)
{
    CHECK_ON_VALID_INT;

    rapidjson::Value::ConstMemberIterator it = document.FindMember(key1);

    if (it->value.GetType() == rapidjson::Type::kObjectType)
    {
        if (it->value.HasMember(key2))
        {
            it = it->value.FindMember(key2);

            if (it->value.IsInt())
            {
                return it->value.GetInt();
            }
        }
    }

    LOG_ERROR("Can't find value for \"%s\" \"%s\"", key1, key2);

    return -1;
}


pchar FileJSON::GetStringValue(pchar key)
{
    CHECK_ON_VALID_STRING;

    rapidjson::Value::ConstMemberIterator it = document.FindMember(key);

    if (it != document.MemberEnd() && it->value.IsString())
    {
        return it->value.GetString();
    }

    LOG_ERROR("Can't find value for \"%s\" key in file \"%s\"", key, full_path.c_str());

    return "";
}


pchar FileJSON::GetStringValue(pchar key1, pchar key2)
{
    CHECK_ON_VALID_STRING;

    rapidjson::Value::ConstMemberIterator it = document.FindMember(key1);

    if (it->value.GetType() == rapidjson::Type::kObjectType)
    {
        if (it->value.HasMember(key2))
        {
            it = it->value.FindMember(key2);

            if (it->value.IsString())
            {
                return it->value.GetString();
            }
        }
    }

    LOG_ERROR("Can't find value for \"%s\" \"%s\"", key1, key2);

    return "";
}


pchar FileJSON::GetStringValue(pchar key1, pchar key2, pchar key3)
{
    CHECK_ON_VALID_STRING;

    rapidjson::Value::ConstMemberIterator it = document.FindMember(key1);

    if (it->value.GetType() == rapidjson::Type::kObjectType)
    {
        it = it->value.FindMember(key2);

        if (it->value.GetType() == rapidjson::Type::kObjectType)
        {
            if (it->value.HasMember(key3))
            {
                it = it->value.FindMember(key3);

                if (it->value.IsString())
                {
                    return it->value.GetString();
                }
            }
        }
    }

    LOG_ERROR("Can't find value for \"%s\" \"%s\" \"%s\"", key1, key2, key3);

    return "";
}


bool FileJSON::GetVectorStrings(pchar key, wxArrayString &strings)
{
    strings.Clear();

    if (!is_valid)
    {
        LOG_ERROR("Configuration file is not valid");
        return false;
    }

    if (document.HasMember(key))
    {
        rapidjson::Value::ConstMemberIterator it = document.FindMember(key);

        auto &value = it->value;

        if (value.IsArray() && value.Size() > 0)
        {
            for (rapidjson::Value::ConstValueIterator elem = value.Begin(); elem != value.End(); ++elem)
            {
                if (elem->IsString())
                {
                    strings.Add(elem->GetString());
                }
            }
        }
    }

    return strings.GetCount() != 0;
}
