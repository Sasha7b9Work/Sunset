// 2025/08/28 21:58:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Tests/Model.h"


namespace Model
{
    static bool empty = true;
    static wxString name = "";
}


bool Model::IsEmpty()
{
    return empty;
}


void Model::CreateNew(const wxString &_name)
{
    empty = false;
    name = _name;
}


void Model::Delete()
{
    empty = true;
    name.Clear();
}


wxString Model::GetName()
{
    return name;
}


bool Model::IsModified()
{
    return true;
}
