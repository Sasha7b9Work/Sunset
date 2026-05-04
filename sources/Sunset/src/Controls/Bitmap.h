// 2025/08/10 13:41:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/bitmap.h>
#pragma warning(pop)
#include <map>


class Bitmap
{
public:

    Bitmap() = default;

    Bitmap(const wxString &file_name);

    static Bitmap &Get(const wxString &file_name);

    wxBitmap &GetBitmap();

private:

    wxBitmap bitmap;

    static std::map<wxString, Bitmap> bitmaps;
};
