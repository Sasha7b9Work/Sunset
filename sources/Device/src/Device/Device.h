// 2025/11/27 15:47:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Device
{
    void Init();

    void PeriodicTask();

    // По этой команде засылаем во все регистры и DACи нули - чтобы исключить появление
    // высокого напряжения во всех возможных местах схемы
    void EmergencyStop();

    // По этой команде в регистры записываются значения, соотвествующие установленным настройкам
    void EmergencyStart();
}
