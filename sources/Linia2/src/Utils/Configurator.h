// 2023/08/11 17:01:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Color.h"


namespace Config
{
    void Init();

    void Flush();

    void DeInit();

    void ReadFontParameter(const wxString &, wxFontFamily &);
    void ReadFontParameter(const wxString &, wxFontStyle &);
    void ReadFontParameter(const wxString &, wxFontWeight &);

    // Устанавливает файл с настройками.
    // Если "", будет установлен глобальный файл
    void SetFile(const wxString &file_path);

    int ReadInt(const wxString &key, int def = 0);
    int Read(const wxString &key, int def = 0);

    uint64 ReadUInt64(const wxString &key, uint64 def = 0);
    BitSet128 ReadUInt128(const wxString &key, const BitSet128 &def = BitSet128());

    int64 ReadInt64(const wxString &key, int64 def = 0);

    bool ReadBool(const wxString &key, bool def = false);
    bool Read(const wxString &key, bool def = false);

    wxString ReadString(const wxString &key, const wxString &def = "");
    wxString Read(const wxString &key, const wxString &def = "");

    void WriteUint(const wxString &key, uint);
    uint ReadUint(const wxString &key, uint def = 0);
    uint Read(const wxString &key, uint &);
    void Write(const wxString &key, uint &);

    void WriteInt(const wxString &key, int);
    void Write(const wxString &key, int);

    void WriteUInt64(const wxString &key, uint64);
    void WriteUInt128(const wxString &key, const BitSet128 &);

    void WriteInt64(const wxString &key, int64);

    void WriteBool(const wxString &key, bool);

    void WriteString(const wxString &key, const wxString &);
    void Write(const wxString &key, const wxString &);

    wxPoint Read(const wxString &key, const wxPoint &def = { 0, 0 });
    void Write(const wxString &key, const wxPoint &value);

    void Write(const wxString &key, const Color &value);
    Color Read(const wxString &key, const Color &def);
}
