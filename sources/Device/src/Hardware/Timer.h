// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


 // @brief Количество тиков, прошедших с момента последнего вызова функции Timer_StartMultiMeasurement(). Не более (1 << 32)
 // В одной секунде 120.000.000 тиков для С8-53 и 90.000.000 тиков для С8-54.
 // Максимальный отрезок времени, который можно отсчитать с её помощью - 35 сек.
#define TIME_TICKS (HAL_TIM2::GetTicks())
#define TIME_US    (HAL_TIM2::TimeUS())
#define TIME_MS    (HAL_TIM::TimeMS())

namespace Timer
{
    struct Type
    {
        enum E 
        {
            kPressKey,                  // Нужно устанавливать, когда приходит нажатие клавиши.
            kShowLevelRShiftA,          // Нужно устанавливать, когда изменяется положение ручки смещения канала 1.
            kShowLevelRShiftB,          // Нужно устанавливать, когда изменяется положение ручки смещения канала 2.
            kShowLevelTrigLev,          // Нужно устанавливать, когда изменяется положение ручки уровня синхронизации
            kENumSignalsInSec,          // Для установки количества считываний сигнала в секунду.
            kFlashDisplay,              // Таймер для мерцающих участков экрана чёрно-белого дисплея.
            kShowMessages,              // Таймер для засекания время показа информационных и предупреждающих сообщений.
            kMenuAutoHide,              // Таймер для отсчёта времени скрывания меню.
            kRShiftMarkersAutoHide,     // Таймер для отсчёта времени скрывания дополнительных боковых меток.
            kUSB,                       // Вспомогательный, для общих нужд.
            kStopSound,                 // Выключить звук
            kTemporaryPauseFPGA,        // Временная пауза для фиксации сигнала на экране после поворота ручек
            kStrNaviAutoHide,           // Прятать строку навигации меню
            kTimerStartP2P,             // Таймер для запуска следующего цикла поточечного чтения
            kTimerDisplay,              // Таймер на ручную отрисовку экрана
            USB,
            kTemp,
            Count
        } value;
        Type(E v) : value(v) {}
        operator uint8() const { return (uint8)value; }
    };

    void Init();

    // Назначает таймеру timer функцию и время срабатывания
    void Set(Type type, pFuncVV func, uint dTms);

    void SetAndStartOnce(Type type, pFuncVV func, uint dTms);

    void SetAndEnable(Type type, pFuncVV func, uint dTms);

    void StartOnce(Type type);

    void Enable(Type type);

    void Disable(Type type);

    bool IsRun(Type type);

    void PauseOnTicks(uint numTicks);

    // Устанавливает стартовую точку логгирования. Далее вызовы Timer_LogPoint засекают временные интервалы от это точки
    void StartLogging();

    uint LogPointUS(char *name);

    uint LogPointMS(char *name);

    // Вызывается при срабатывании таймера
    void ElapsedCallback();

    void DelayMS(uint delayMS);
};


struct TimeMeterMS
{
    TimeMeterMS() { Reset(); }
    void Reset();
    uint ElapsedMS();
private:
    uint time_reset;
};


// !!! Внимание !!! Эти измерения действительны в пределах одного прохода главного цикла программы
struct TimeMeterUS
{
    TimeMeterUS()
    {
        Reset();
    }

    void Reset();

    uint ElapsedUS() const;

    // Ожидать, пока не пройдёт timeUS микросекунд после последнего reset
    void WaitForUS(uint timeUS);

    // Ожидать timeUS после последнего сброса и сбросить
    void WaitForUSandReset(uint timeUS);

private:

    uint time_reset = 0;
};
