// 2025/08/10 13:41:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Bitmap.h"
#pragma warning(push, 0)
#include <wx/filename.h>
#pragma warning(pop)


std::map<wxString, Bitmap> Bitmap::bitmaps;


Bitmap::Bitmap(const wxString &file_bitmap)
{
    wxString path = wxString("resources/") + file_bitmap;

    wxFileName filename(path);

    if (file_bitmap[file_bitmap.size() - 1] == 'p')
    {
        wxImage image;

        if (filename.FileExists() && image.LoadFile(path, wxBITMAP_TYPE_BMP))
        {
            bitmap = wxBitmap(image);

            bitmap.SetMask(new wxMask(bitmap, *wxWHITE));
        }
        else
        {
            LOG_ERROR(_("Не удалось загрузить файл изображения %s"), file_bitmap.c_str().AsChar());
        }
    }
    else
    {
        wxIcon icon;

        if (filename.FileExists() && icon.LoadFile(path, wxBITMAP_TYPE_ICO))
        {
            bitmap.CopyFromIcon(icon);
        }
        else
        {
            LOG_ERROR(_("Не удалось загрузить файл изображения %s"), file_bitmap.c_str().AsChar());
        }
    }
}


Bitmap &Bitmap::Get(const wxString &file_name)
{
    if (bitmaps.find(file_name) == bitmaps.end())
    {
        bitmaps[file_name] = Bitmap(file_name);
    }

    Bitmap &result = bitmaps[file_name];

    if (!result.GetBitmap().IsOk())
    {
        LOG_ERROR("Can not load image %s", file_name.c_str().AsChar());
    }

    return result;
}


wxBitmap &Bitmap::GetBitmap()
{
    return bitmap;
}
