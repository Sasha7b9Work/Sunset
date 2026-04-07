// 2025/6/3 14:02:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelDebug/Notebook/Register.h"


// Это панель для засылки данных в регистры микросхем типа AD

class PageChip : public wxPanel
{
public:

    PageChip(wxNotebook *parent, const wxString &title);

    void AppendRegister(Register *);

    void Pack();
    void Unpack();

    void Rebuild();

    // Заслать ве значения регистров в прибор
    void SendAllRegisters();

private:

    wxMenuItem *itemReturn = nullptr;

    std::vector<Register *> registers;

    void OnRightClick(wxMouseEvent &);

    void OnMenuEvent(wxCommandEvent &);
};
