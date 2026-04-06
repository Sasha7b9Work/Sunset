// 2025/08/28 21:57:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Model
{
    // Нет модели
    bool IsEmpty();

    void CreateNew(const wxString &name);

    void Delete();

    wxString GetName();

    bool IsModified();
}
