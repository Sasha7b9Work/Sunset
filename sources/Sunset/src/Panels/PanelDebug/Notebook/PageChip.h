// 2025/6/3 14:02:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Panel.h"
#pragma warning(push, 0)
#include <wx/notebook.h>
#pragma warning(pop)


class Register;


// Это панель для засылки данных в регистры микросхем типа AD

class PageChip : public Panel
{
public:

    PageChip(wxNotebook *, const wxString &title);

    void AppendRegister(Register *);

    void Pack();
    void Unpack();

    // Заслать ве значения регистров в прибор
    void SendAllRegisters();

private:

    wxMenuItem *itemReturn = nullptr;

    std::vector<Register *> registers;

    void OnRightClick(wxMouseEvent &);

    void OnMenuEvent(wxCommandEvent &);
};
