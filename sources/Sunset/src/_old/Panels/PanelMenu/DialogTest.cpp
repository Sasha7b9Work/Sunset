// 2025/7/12 15:42:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/DialogTest.h"


DialogTest *TheDialogTest = nullptr;
DialogTestAppend *DialogTestAppend::self = nullptr;
DialogTestLibrary *DialogTestLibrary::self = nullptr;


const wxString DialogTest::BTN_APPEND = wxT("Добавить");
const wxString DialogTest::BTN_SAVE_PARAMETERS = wxT("Сохранить параметры");
const wxString DialogTest::BTN_DELETE = wxT("Удалить");
const wxString DialogTest::BTN_DELETE_ALL = wxT("Удалить все");
const wxString DialogTest::BTN_COPY = wxT("Копировать");
const wxString DialogTest::BTN_INSERT = wxT("Вставить");
const wxString DialogTest::BTN_RENAME = wxT("Переименовать");
const wxString DialogTest::BTN_LIBRARY = wxT("Библиотека тестов");
const wxString DialogTest::BTN_SAVE_TO_LIBRARY = wxT("Сохранить в библиотеке");

const wxString DialogTestAppend::BTN_ACTIVE = wxT("Активный");
const wxString DialogTestAppend::BTN_FROM_LIBRARY = wxT("Из библиотеки");

const wxString DialogTestLibrary::BTN_STANDARD = wxT("Стандартных");
const wxString DialogTestLibrary::BTN_USER = wxT("Пользовательских");


DialogTest::DialogTest() :
    MenuDialog(wxT("Тесты"), 175, { 0, 1, 3, 6, 7 },
        BTN_APPEND, []()
        {
            DialogTestAppend().ShowOnWindow(TheDialogTest->FindButton(BTN_APPEND));
        },
        BTN_SAVE_PARAMETERS, []()
        {
        },
        BTN_DELETE, []()
        {
        },
        BTN_DELETE_ALL, []()
        {
        },
        BTN_COPY, []()
        {
        },
        BTN_INSERT, []()
        {
        },
        BTN_RENAME, []()
        {
        },
        BTN_LIBRARY, []()
        {
            DialogTestLibrary().ShowOnWindow(TheDialogTest->FindButton(BTN_LIBRARY));
        },
        BTN_SAVE_TO_LIBRARY, []()
        {
        }
    )
{
    TheDialogTest = this;

    UpdateStateControls();
}


void DialogTest::UpdateStateControls()
{
    FindButton(BTN_SAVE_PARAMETERS)->Enable(false);
    FindButton(BTN_DELETE)->Enable(false);
    FindButton(BTN_DELETE_ALL)->Enable(false);
    FindButton(BTN_COPY)->Enable(false);
    FindButton(BTN_INSERT)->Enable(false);
    FindButton(BTN_RENAME)->Enable(false);
}


DialogTestAppend::DialogTestAppend() :
    MenuDialog(DialogTest::BTN_APPEND, 150, { },
        BTN_ACTIVE, []()
        {
        },
        BTN_FROM_LIBRARY, []()
        {
        }
    )
{
    self = this;
}


DialogTestLibrary::DialogTestLibrary() :
    MenuDialog(DialogTest::BTN_LIBRARY, 150, { },
        BTN_STANDARD, []()
        {
        },
        BTN_USER, []()
        {
        }
    )
{
    self = this;
}
