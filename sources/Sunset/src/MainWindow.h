// 2023/07/04 17:46:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Value.h"
#include "Panels/PanelBoard.h"


struct ModeMainWindow
{
    friend class MainWindow;

    enum E
    {
        Standard,           // Стартовый вид окна
        ReferenceGraphs,    // Архив эталонных графиков на весь экран
        FullGraph,          // Графики на весь экран
        Debug,              // Отладочный режим - засылка в регисты
        Count
    };

    static E Current()
    {
        return current;
    }

private:

    static E current;
};


class MainWindow : public wxFrame
{
    friend class Application;

public:

    static const int WIDTH = 1024;     // Полная ширина - соотвествует разрешению экрана
    static const int HEIGHT = 768;     // Полная высота - соотвествует разрешению экрана

    static const int WIDTH1 = 160;                  // Ширина левой верхней зоны
    static const int WIDTH_DRAW = 820;              // Ширина зоны отрисовки
    static const int WIDTH3 = WIDTH - WIDTH_DRAW;   // Ширина последнего, третьего, столбика с панелями

    static const int HEIGHT_HI = 75;
    static const int HEIGHT_DRAW = HEIGHT - HEIGHT_HI;

    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    void OnEventCloseWindow(wxCloseEvent &);

    void SetMode(ModeMainWindow::E);

    void PeriodicTask();

    // Спрятать системную панель задач. Нужно вызывать после создания диалоговых окон чтобы спрятать системную панель.
    void HideSystemPanel();

private:

    MainWindow(const wxString &title);

    wxToolBar *toolBar = nullptr;

    ValuePoint position{ "gui_position", { 100, 100 } };

    PanelBoard *board = nullptr;

    void OnEventKeyHook(wxKeyEvent &);
    void OnEventMaximize(wxMaximizeEvent &);

    // Настроить шрифт
    void TuneFont();

    void SetPosition();
};
