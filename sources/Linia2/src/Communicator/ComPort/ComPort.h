// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Используется в Windows-версии для связи с платой контроллера


namespace ComPort
{
    // Возвращает массив портов. 0 == COM1, true - cуществует
    void GetComports(std::vector<bool> &);

    // 0 == COM1
    bool Connect(int num_port);

    void Disconnect();

    void Send(pchar);

    void Send(void *buffer, int num_bytes);

    // Ожидать, пока в ответе слово под номером num_word будет word
    void WaitWord(int num_word, pchar word);

    pchar NameOpenedPort();

    namespace CallbackOnReceive
    {
        void Set(void (*callback)(pchar));

        void Reset();
    }
};
