// 2025/7/12 16:18:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Panels/PanelMenu/Setup/DialogSetup.h"
#include "Panels/PanelMenu/Setup/WindowCorrectionZero.h"
#include "Panels/PanelMenu/Setup/WindowCalibrate.h"
#include "Panels/PanelMenu/Setup/WindowTableOffsets.h"
#include "Panels/PanelMenu/Setup/WindowMyComputer.h"
#include "Panels/PanelMenu/Setup/WindowUser.h"
#include "Panels/PanelMenu/Setup/WindowLaunch.h"
#include "Panels/PanelMenu/Setup/WindowAutoSave.h"
#include "Panels/PanelMenu/Setup/WindowPassword.h"
#include "Panels/PanelMenu/Setup/WindowAddressIP.h"


DialogSetup *DialogSetup::self = nullptr;


#define SET_USER_SETTINGS wxT("Настройки пользователя")
#define SET_START         wxT("Запуск")
#define SET_AUTOSAVE      wxT("Автосохранение")
#define SET_CORR_ZERO     wxT("Коррекция смещения нуля")
#define SET_CALIB         wxT("Калибровка")
#define SET_TABLES_OFF    wxT("Таблица смещений и коэффициентов")
#define SET_PASSWORD      wxT("Изменить пароль")
#define SET_IP            wxT("IP-адрес")
#define SET_COMPUPTER     wxT("Мой компьютер")


DialogSetup::DialogSetup() :
    MenuDialog(wxT("Настройки"), 275, { 2, 5 },
        SET_USER_SETTINGS, []()
        {
            WindowUser().ShowOnWindow(DialogSetup::self->FindButton(SET_USER_SETTINGS));
        },
        SET_START, []()
        {
            WindowLaunch().ShowOnWindow(DialogSetup::self->FindButton(SET_START));
        },
        SET_AUTOSAVE, []()
        {
            WindowAutoSave().ShowOnWindow(DialogSetup::self->FindButton(SET_AUTOSAVE));
        },
        SET_CORR_ZERO, []()
        {
            WindowCorretionZero().ShowOnWindow(DialogSetup::self->FindButton(SET_CORR_ZERO));
        },
        SET_CALIB, []()
        {
            WindowCalibrate().ShowOnWindow(DialogSetup::self->FindButton(SET_CALIB));
        },
        SET_TABLES_OFF, []()
        {
            WindowTableOffsets().ShowOnWindow(DialogSetup::self->FindButton(SET_TABLES_OFF));
        },
        SET_PASSWORD, []()
        {
            WindowPassword().ShowOnWindow(DialogSetup::self->FindButton(SET_PASSWORD));
        },
        SET_IP, []()
        {
            WindowAddressIP().ShowOnWindow(DialogSetup::self->FindButton(SET_IP));
        },
        SET_COMPUPTER, []()
        {
            WindowMyComputer().ShowOnWindow(DialogSetup::self->FindButton(SET_COMPUPTER));
        }
    )
{
    self = this;
}

