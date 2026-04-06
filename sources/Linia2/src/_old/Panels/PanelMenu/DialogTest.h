// 2025/7/12 15:43:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelMenu/MenuDialog.h"


class DialogTest : public MenuDialog
{
public:

    DialogTest();

    void UpdateStateControls();

    static const wxString BTN_APPEND;
    static const wxString BTN_LIBRARY;

private:

    static const wxString BTN_SAVE_PARAMETERS;
    static const wxString BTN_DELETE;
    static const wxString BTN_DELETE_ALL;
    static const wxString BTN_COPY;
    static const wxString BTN_INSERT;
    static const wxString BTN_RENAME;
    static const wxString BTN_SAVE_TO_LIBRARY;
};


class DialogTestAppend : public MenuDialog
{
public:

    DialogTestAppend();

    static DialogTestAppend *self;

private:

    static const wxString BTN_ACTIVE;
    static const wxString BTN_FROM_LIBRARY;
};


class DialogTestLibrary : public MenuDialog
{
public:

    DialogTestLibrary();

    static DialogTestLibrary *self;

private:

    static const wxString BTN_STANDARD;
    static const wxString BTN_USER;
};
