// 2026/03/13 16:31:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "MainPanel/PanelMeasures/MenuDisplay.h"
#include "MainPanel/PanelMeasures/Grid/Grid.h"
#include "MainPanel/PanelMeasures/PanelMeasures.h"
#include "MainWindow.h"
#include "Settings/Settings.h"
#include "MainPanel/PanelMeasures/Graphics/AutoCursors.h"
#include "MainPanel/PanelMeasures/Graphics/GraphMeasure.h"
#pragma warning(push, 0)
#include <wx/colourdata.h>
#include <wx/colordlg.h>
#pragma warning(pop)


MenuDisplay::MenuDisplay() : wxMenu()
{
    TheAutoCursors->Ban();

    Bind(wxEVT_MENU, &MenuDisplay::OnResetZeroGrid, this, (Append(wxID_ANY, "Сброс"))->GetId());

    wxMenuItem *item = AppendCheckItem(wxID_ANY, "Полный экран");
    Bind(wxEVT_MENU, &MenuDisplay::OnFullScreen, this, item->GetId());
    Check(item->GetId(), ThePanelMeasures->full_screen);

    AppendSeparator();

    AppendMenuTrack();

    AppendMenuFacade();

    AppendSeparator();

//    Bind(wxEVT_MENU, &MenuDisplay::OnSaveGraphic, this, (Append(wxID_ANY, "Сохранить график"))->GetId());
//    Bind(wxEVT_MENU, &MenuDisplay::OnResetGraphic, this, (Append(wxID_ANY, "Сбросить график"))->GetId());
}


MenuDisplay::~MenuDisplay()
{
    TheAutoCursors->Allow();
}


void MenuDisplay::AppendMenuTrack()
{
    wxMenu *subMenu = new wxMenu();

    trackX = subMenu->AppendCheckItem(wxID_ANY, "По X");
    Bind(wxEVT_MENU, &MenuDisplay::OnTrack, this, trackX->GetId());
//    subMenu->Check(trackX->GetId(), TheWindowCursors->chbTrackX->IsChecked());

    trackY = subMenu->AppendCheckItem(wxID_ANY, "По Y");
    Bind(wxEVT_MENU, &MenuDisplay::OnTrack, this, trackY->GetId());
//    subMenu->Check(trackY->GetId(), TheWindowCursors->chbTrackY->IsChecked());

    trackMouse = subMenu->AppendCheckItem(wxID_ANY, "Указатель мыши");
    Bind(wxEVT_MENU, &MenuDisplay::OnTrack, this, trackMouse->GetId());
//    subMenu->Check(trackMouse->GetId(), TheWindowCursors->chbTrackMouse->IsChecked());

    AppendSubMenu(subMenu, "Отслеживать");
}


void MenuDisplay::AppendMenuFacade()
{
    wxMenu *subFacade = new wxMenu();

    {
        // Настройка цветов

        wxMenuItem *item = nullptr;
        StructColor str_color;

#define APPEND_COLOR(title, value_color, func)                          \
        item = subColors->Append(wxID_ANY, title);                      \
        Bind(wxEVT_MENU, &MenuDisplay::OnColor, this, item->GetId());   \
        str_color = { value_color, func };                             \
        colors[item->GetId()] = str_color;

        wxMenu *subColors = new wxMenu();

        APPEND_COLOR(wxT("Фон"), SET::GUI::color_background, nullptr);
        APPEND_COLOR(wxT("Сетка"), SET::GUI::color_grid, nullptr);
        APPEND_COLOR(wxT("Шрифт"), SET::GUI::color_font, nullptr);
        APPEND_COLOR(wxT("Кривая"), SET::GUI::color_curve, OnColorCurve);
        APPEND_COLOR(wxT("Ссылка"), SET::GUI::color_link, nullptr);
        APPEND_COLOR(wxT("Секущая"), SET::GUI::color_secant, nullptr);
        subColors->AppendSeparator();
        APPEND_COLOR(wxT("Сбросить"), nullptr, nullptr);

        subFacade->AppendSubMenu(subColors, "Цвета");
    }

    {
        wxMenuItem *item = nullptr;

#define APPEND_SIZE(title)                                                  \
        item = subSize->AppendRadioItem(wxID_ANY, title);                   \
        Bind(wxEVT_MENU, &MenuDisplay::OnSizePoint, this, item->GetId());

        wxMenu *subSize = new wxMenu();

        APPEND_SIZE(wxT("Откл"));
        APPEND_SIZE("2");
        APPEND_SIZE("3");
        APPEND_SIZE("4");
        APPEND_SIZE("5");

        subFacade->AppendSubMenu(subSize, "Размер точки");

        for (auto it = subSize->GetMenuItems().begin(); it != subSize->GetMenuItems().end(); ++it)
        {
            item = *it;

            int size = 0;

            item->GetItemLabelText().ToInt(&size);

            if (size == SET::GUI::size_point->Get())
            {
                item->Check(true);
            }
        }
    }

    AppendSubMenu(subFacade, "Внешний вид");
}


void MenuDisplay::OnResetZeroGrid(wxCommandEvent &)
{
    TheGrid->Reset();
}


void MenuDisplay::OnFullScreen(wxCommandEvent &event)
{
    TheMainWindow->SetMode(event.IsChecked() ? ModeMainWindow::FullGraph : ModeMainWindow::Standard);

    TheGrid->ResetCenter();

    ThePanelMeasures->Refresh();
}


void MenuDisplay::OnTrack(wxCommandEvent &event)
{
    int id = event.GetId();
//    bool check = event.IsChecked();

    if (event.GetId() == trackX->GetId())
    {
//        TheWindowCursors->chbTrackX->SetNewValue(check);
    }
    else if(id == trackY->GetId())
    {
//        TheWindowCursors->chbTrackY->SetNewValue(check);
    }
    else if (id == trackMouse->GetId())
    {
//        TheWindowCursors->chbTrackMouse->SetNewValue(check);
    }
}


void MenuDisplay::OnColor(wxCommandEvent &event)
{
    auto SetColor = [](const wxString &title, wxColour &color) -> bool
        {
            wxColourData colourData;
            colourData.SetChooseFull(true);

            wxColourDialog dialog(ThePanelMeasures, &colourData);

            dialog.SetTitle(wxString(wxT("Цвет ")) + title);

            if (dialog.ShowModal() == wxID_OK)
            {
                color = dialog.GetColourData().GetColour();

                return true;
            }

            return false;
        };

    wxMenuItem *item = FindItem(event.GetId());

    if (item)
    {
        wxColour color;

        auto elem = colors.find(event.GetId());

        if (elem != colors.end())
        {
            if (elem->second.set)
            {
                if (SetColor(item->GetItemLabel(), color))
                {
                    elem->second.set->Set(color.GetRGB());

                    if (elem->second.func)
                    {
                        elem->second.func();
                    }
                }
            }
            else                            // Сбросить
            {
                for (auto col : colors)
                {
                    if (col.second.set)
                    {
                        col.second.set->Reset();
                        OnColorCurve();
                    }
                }
            }
        }

    }
}


void MenuDisplay::OnSizePoint(wxCommandEvent &event)
{
    wxMenuItem *item = FindItem(event.GetId());

    if (item)
    {
        wxString title = item->GetItemLabelText();

        int value = 0;

        title.ToInt(&value, 10);

        SET::GUI::size_point->Set(value);

        ThePanelMeasures->Refresh();
    }
}


void MenuDisplay::OnSaveGraphic(wxCommandEvent &)
{

}


void MenuDisplay::OnResetGraphic(wxCommandEvent &)
{

}


void MenuDisplay::OnColorCurve()
{
    for (auto elem : ThePanelMeasures->entities)
    {
        elem->SetColor(SET::GUI::color_curve->Get());
    }
}
