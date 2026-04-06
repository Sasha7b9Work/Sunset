// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/ComPort/ComPort.h"
#include "Communicator/ComPort/rs232.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"
#include "Windows/ConsoleRS232.h"


using namespace std::chrono;


namespace ComPort
{
    static int connected_port = -1;     // 0 == COM1

    static String lastAnswer;

    static void (*callbackOnReceive)(pchar) = nullptr;

    static wxString ExtractMessage(char *);


    // Столько первых портов опрашивается
    static const int NUM_PORTS = 32;

    // Каждофреймовое обновление при подключённом устройстве
    void UpdateConnected();

    bool IsConnected();

    int Receive(char *buffer, int size);
}


void ComPort::GetComports(std::vector<bool> &ports)
{
    ports.clear();

    for (int i = 0; i < NUM_PORTS; i++)
    {
        static const pchar mode = "8N1";

        if (RS232_OpenComport(i, 115200, mode, 0) == 0)
        {
            ports.push_back(true);

            RS232_CloseComport(i);
        }
        else
        {
            ports.push_back(false);
        }
    }
}


bool ComPort::Connect(int num_port)
{
#ifdef WIN32

    Disconnect();

    if (num_port < 0)
    {
        return false;
    }

    if (RS232_OpenComport(num_port, 115200, "8N1", 0) == 0)     // Открываем порт с меньшим на 1 значением - в функциях rs232.cpp нумерация начинается с нуля
    {
        connected_port = num_port;

        SET::GUI::serial_port_num->Set(num_port + 1);

//        PanelUpper::self->color_square->SetColor(*wxGREEN);

        return true;
    }

//    PanelUpper::self->color_square->SetColor(*wxRED);

#endif

    return false;
}


pchar ComPort::NameOpenedPort()
{
    static char buffer[32];

    std::sprintf(buffer, "COM%d", connected_port + 1);

    return buffer;
}


void ComPort::Disconnect()
{
    if (IsConnected())
    {
        RS232_CloseComport(connected_port);

        connected_port = -1;
    }
}


bool ComPort::IsConnected()
{
    return connected_port != -1;
}


void ComPort::Send(pchar buffer)
{
    if (IsConnected())
    {
        Send((void *)buffer, (int)std::strlen(buffer));
    }
}


void ComPort::Send(void *buffer, int num_bytes)
{
    if (IsConnected())
    {
        RS232_SendBuf(connected_port, (uint8 *)buffer, num_bytes);
    }
}


int ComPort::Receive(char *buffer, int size)
{
    if (IsConnected())
    {
        return RS232_PollComport(connected_port, reinterpret_cast<unsigned char *>(buffer), size);
    }

    return 0;
}


void ComPort::UpdateConnected()
{
    if (IsConnected())
    {
        const int SIZE_BUFFER = 4096;
        static char buffer[SIZE_BUFFER] = { 0 };  // Принятые данные
        static int bytes_in_buffer = 0;

        static bool first = true;

        if (first)
        {
            first = false;
            std::memset(buffer, 0xFF, SIZE_BUFFER);
        }

        int n = ComPort::Receive(buffer + bytes_in_buffer, SIZE_BUFFER - bytes_in_buffer);

        if (n)
        {
            bytes_in_buffer += n;

            while (true)
            {
                wxString message = ExtractMessage(buffer);

                if (message.Length())
                {
                    lastAnswer.Set(message.c_str());

                    if (lastAnswer.GetWord(1, " ") != "Task")
                    {
                        if (message != "#OK")                       // Подтверждения отбрасываем, чтобы не засорять лог
                        {
                            ConsoleRS232::self->AddLine(String(_("%s%s"), REVERSE_PROMT, buffer).c_str());

                            Log::FromDevice(buffer);
                        }

                        if (callbackOnReceive)
                        {
                            callbackOnReceive(buffer);
                        }
                    }

                    int remove_bytes = (int)message.Length() + 1;
                    bytes_in_buffer -= remove_bytes;

                    if (bytes_in_buffer)
                    {
                        std::memmove(buffer, buffer + remove_bytes, (uint)bytes_in_buffer);
                    }

                    std::memset(buffer + bytes_in_buffer, 0xFF, (uint)(SIZE_BUFFER - bytes_in_buffer));
                }
                else
                {
                    break;
                }
            }
        }
    }
}


wxString ComPort::ExtractMessage(char *buffer)
{
    if (SU::BeginWith(buffer, "#OK") ||
        SU::BeginWith(buffer, "#CARD") ||
        SU::BeginWith(buffer, "#LOG") ||
        SU::BeginWith(buffer, "#WARN") ||
        SU::BeginWith(buffer, "#ERROR") ||
        SU::BeginWith(buffer, "#DATA") ||
        SU::BeginWith(buffer, "#MAKE") ||
        SU::BeginWith(buffer, "#KEYSMFSL0"))
    {
        for (int i = 0; ; i++)
        {
            if (buffer[i] == '\n')
            {
                buffer[i] = '\0';

                return wxString(buffer);
            }

            if (buffer[i] == -1)
            {
                return "";
            }
        }
    }
    else
    {
        pchar pointer = SU::Consist(buffer, "#END\n");

        if (pointer)
        {
            int64 index = pointer - buffer + 4;
            buffer[index] = '\0';
            return wxString(buffer);
        }
    }

    return "";
}


void ComPort::WaitWord(int num_word, pchar word)
{
    while (lastAnswer.GetWord(num_word, " ") != word)
    {
        UpdateConnected();
    }

    lastAnswer.Set("");
}


void ComPort::CallbackOnReceive::Set(void (*callback)(pchar))
{
    callbackOnReceive = callback;
}


void ComPort::CallbackOnReceive::Reset()
{
}
