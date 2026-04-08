// 2026/03/17 10:46:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Application;
struct AutoCursors;
class DialogTest;
class IGrid;
class MainWindow;
class NotebookDebug;
class WindowCursors;

extern Application   *TheApp;
extern AutoCursors   *TheAutoCursors;
extern DialogTest    *TheDialogTest;
extern IGrid         *TheGrid;
extern MainWindow    *TheMainWindow;
extern NotebookDebug *TheNotebookDebug;
extern WindowCursors *TheWindowCursors;     // Настройки курсоров

struct PanelMeasures;
struct PanelScheme;
struct PanelRight;
struct PanelDebug;

extern PanelMeasures *ThePanelMeasures;
extern PanelScheme  *ThePanelScheme;
extern PanelRight   *ThePanelRight;
extern PanelDebug   *ThePanelDebug;
