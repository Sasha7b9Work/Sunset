// 2023/08/11 17:01:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Configurator.h"


namespace Config
{
    static wxConfigBase *base = nullptr;
    static wxFileConfig *file = nullptr;
}


void Config::Init()
{
    wxString file_name_config = wxGetCwd() + "/Linia.conf";

    file = new wxFileConfig("", "", file_name_config);

    SetFile("");
}


void Config::Flush()
{
    wxConfigBase::Get(false)->Flush();
}


void Config::DeInit()
{
    wxConfigBase::Get(false)->Flush();

    wxConfigBase::Set(nullptr);

    SAFE_DELETE(file);
}


void Config::SetFile(const wxString &file_path)
{
    static wxString current_file = "unimaginable name";

    if (current_file == file_path)
    {
        return;
    }

    current_file = file_path;

    static wxFileConfig *config = nullptr;

    if (config)
    {
        SAFE_DELETE(config);
    }

    if (file_path.IsEmpty())
    {
        wxConfigBase::Set(file);
        Config::base = wxConfigBase::Get(false);
    }
    else
    {
        config = new wxFileConfig("IPPP", "MNIPI", file_path);

        wxConfigBase::Set(config);
        Config::base = wxConfigBase::Get(false);
    }
}


int Config::ReadInt(const wxString &key, int def)
{
    int result = def;

    Config::base->Read(key, &result, def);

    return result;
}


int Config::Read(const wxString &key, int def)
{
    return ReadInt(key, def);
}


wxPoint Config::Read(const wxString &key, const wxPoint &def)
{
    int x = ReadInt(key + "_x", def.x);
    int y = ReadInt(key + "_y", def.y);

    return { x, y };
}


void Config::Write(const wxString &key, const wxPoint &value)
{
    WriteInt(key + "_x", value.x);
    WriteInt(key + "_y", value.y);
}


uint64 Config::ReadUInt64(const wxString &key, uint64 def)
{
    uint64 result = def;

    Config::base->Read(key, (int64 *)&result, (int64)def);

    return result;
}


BitSet128 Config::ReadUInt128(const wxString &key, const BitSet128 &def)
{
    BitSet128 result = def;

    Config::base->Read(key + "[0]", (int64 *)&result.word64[0], (int64)def.word64[0]);
    Config::base->Read(key + "[1]", (int64 *)&result.word64[1], (int64)def.word64[1]);

    return result;
}


int64 Config::ReadInt64(const wxString &key, int64 def)
{
    int64 result = def;

    Config::base->Read(key, &result, def);

    return result;
}


bool Config::ReadBool(const wxString &key, bool def)
{
    bool result = def;

    Config::base->Read(key, &result, def);

    return result;
}


bool Config::Read(const wxString &key, bool def)
{
    return ReadBool(key, def);
}


wxString Config::ReadString(const wxString &key, const wxString &def)
{
    wxString result = def;

    Config::base->Read(key, &result, def);

    return result;
}


wxString Config::Read(const wxString &key, const wxString &def)
{
    return ReadString(key, def);
}


void Config::WriteInt(const wxString &key, int value)
{
    Config::base->Write(key, value);
}


void Config::Write(const wxString &key, int value)
{
    WriteInt(key, value);
}


void Config::WriteUint(const wxString &key, uint value)
{
    Config::base->Write(key, value);
}


void Config::Write(const wxString &key, uint &value)
{
    WriteUint(key, value);
}


uint Config::ReadUint(const wxString &key, uint def)
{
    uint result;

    result = (uint)Config::base->Read(key, (long)def);

    return result;
}


uint Config::Read(const wxString &key, uint &def)
{
    return ReadUint(key, def);
}


void Config::Write(const wxString &key, const Color &value)
{
    WriteUint(key, value.value);
}


Color Config::Read(const wxString &key, const Color &def)
{
    Color result;
    result.value = (uint)ReadUInt64(key, def.value);
    return result;
}


void Config::WriteUInt64(const wxString &key, uint64 value)
{
    Config::base->Write(key, (int64)value);
}


void Config::WriteUInt128(const wxString &key, const BitSet128 &value)
{
    Config::base->Write(key + "[0]", (int64)value.word64[0]);
    Config::base->Write(key + "[1]", (int64)value.word64[1]);
}


void Config::WriteInt64(const wxString &key, int64 value)
{
    Config::base->Write(key, value);
}


void Config::WriteBool(const wxString &key, bool value)
{
    Config::base->Write(key, value);
}


void Config::WriteString(const wxString &key, const wxString &value)
{
    Config::base->Write(key, value);
}


void Config::Write(const wxString &key, const wxString &value)
{
    WriteString(key, value);
}


void Config::ReadFontParameter(const wxString &key, wxFontFamily &family)
{
    int value = ReadInt(key, (int)family);

    family = (wxFontFamily)value;
}


void Config::ReadFontParameter(const wxString &key, wxFontStyle &style)
{
    int value = ReadInt(key, (int)style);

    style = (wxFontStyle)value;
}


void Config::ReadFontParameter(const wxString &key, wxFontWeight &weight)
{
    int value = ReadInt(key, (int)weight);

    weight = (wxFontWeight)value;
}
