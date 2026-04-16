// 2026/03/13 16:31:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Value.h"
#pragma warning(push, 0)
#include <wx/menu.h>
#pragma warning(pop)


class MenuDisplay : public wxMenu
{
public:

    MenuDisplay();

    ~MenuDisplay();

private:

    struct StructColor
    {
        Value<uint> *set;
        void (*func)();
    };

    std::map<int, StructColor> colors;      // Здесь будем хранить настройки цветов, соответствующие пунктам меню

    wxMenuItem *trackX = nullptr;
    wxMenuItem *trackY = nullptr;
    wxMenuItem *trackMouse = nullptr;

    // Следить
    void AppendMenuTrack();

    // Внешний вид
    void AppendMenuFacade();

    void OnResetZeroGrid(wxCommandEvent &);
    void OnFullScreen(wxCommandEvent &);
    void OnTrack(wxCommandEvent &);
    void OnColor(wxCommandEvent &);
    void OnSizePoint(wxCommandEvent &);
    void OnSaveGraphic(wxCommandEvent &);
    void OnResetGraphic(wxCommandEvent &);

    static void OnColorCurve();
};
