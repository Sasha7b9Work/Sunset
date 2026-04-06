// 2025/08/31 20:13:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/DataPoint.h"


// Здесь хранится одно измерение - например, семейство ВАХ биполярного транзистора
struct Record
{
    // Открывает запись. Теперь можно добавлять данные
    void Open();

    void AppendPoint(const DataPoint &);

    // Измерение закончено. Закрываем
    void Close();

private:

    // Собрать текущие настройки
    void CollectSettings();
};

