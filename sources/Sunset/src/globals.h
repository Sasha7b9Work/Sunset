// 2026/03/17 10:46:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Application;
class AutoCursors;
class IGrid;
class MainWindow;
class NotebookDebug;

extern Application   *TheApp;
extern AutoCursors   *TheAutoCursors;
extern IGrid         *TheGrid;
extern MainWindow    *TheMainWindow;
extern NotebookDebug *TheNotebookDebug;

class PanelArchive;
class PanelDebug;
class PanelMeasures;
class PanelReports;
class PanelSettings;
class PanelTables;

class PageTests;
class PanelCategory;
class PanelLibrary;
class PanelViewTest;

class PanelRight;

extern PanelDebug    *ThePanelDebug;
extern PanelMeasures *ThePanelMeasures;
extern PanelReports  *ThePanelReports;
extern PanelSettings *ThePanelSettings;
extern PanelTables   *ThePanelTables;

extern PageTests     *ThePageTests;
extern PanelCategory *ThePanelCategory;
extern PanelLibrary  *ThePanelLibrary;
extern PanelViewTest *ThePanelViewTest;

extern PanelRight    *ThePanelRight;
