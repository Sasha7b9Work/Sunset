// 2025/10/09 08:37:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/GPIO/GPIO.h"
#include "Utils/Timer.h"
#include <thread>


// Lin specific
#include <gpiod.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>


PinIn pinDAT_F0(Pin::In_DAT_F0);
PinIn pinDAT_F2(Pin::In_DAT_F2);
PinIn pinFIFO_FULL(Pin::In_FIFO_FULL);

PinIn pinSTART(Pin::In_START);
PinIn pinSTOP(Pin::In_STOP);
PinIn pinKA(Pin::In_KA);
PinIn pinKB(Pin::In_KB);

PinOut pinREQ_RD(Pin::Out_REQ_RD);
PinOut pinOut(Pin::Out);


namespace GPIO
{
    // Отдельные массивы для разных типов пинов
    static InputPinInfo g_input_pins[] = {
        { {  8, "gpiochip1", nullptr, nullptr, "15:GPIO1-B0" }, false, nullptr },  // START     0
        { {  9, "gpiochip1", nullptr, nullptr, "21:GPIO1-B1" }, false, nullptr },  // STOP      1
        { { 13, "gpiochip3", nullptr, nullptr, "16:GPIO3-B5" }, false, nullptr },  // DAT_F0    2
        { { 14, "gpiochip3", nullptr, nullptr, "18:GPIO3-B6" }, false, nullptr },  // SPI MOSI  3
        { {  2, "gpiochip1", nullptr, nullptr, "22:GPIO1-A2" }, false, nullptr },  // DAT_F2    4
        { {  5, "gpiochip3", nullptr, nullptr, "36:GPIO3-A5" }, false, nullptr },  // FIFO_FULL 5

        // \todo Проверить
        { {  4, "gpiochip1", nullptr, nullptr, "11:GPIO1-A4" }, false, nullptr},   // KA        6
        { {  7, "gpiochip1", nullptr, nullptr, "13:GPIO1-A7" }, false, nullptr}    // KB        7
    };

    static OutputPinInfo g_output_pins[] = {
        { {  3, "gpiochip1", nullptr, nullptr, "32:GPIO1-A3" }},    // REQ_RD   0
        { { 12, "gpiochip1", nullptr, nullptr, "24:GPIO1-B4" }},    // SPI CS   1

        // \todo Проверить
        { { 13, "gpiochip1", nullptr, nullptr, "26:GPIO1-B5" }}     // Out      2
    };

    // Маппинг enum Pin::Type на индексы в массивах
    static const struct {
        bool is_input;
        int index;
    } g_pin_mapping[] = {
        { true,  0 },   // START
        { true,  1 },   // STOP
        { true,  2 },   // DAT_F0
        { true,  3 },   // SPI MOSI
        { true,  4 },   // DAT_F2
        { false, 1 },   // Out SPI CS
        { true,  5 },   // FIFO_FULL
        { false, 0 },   // REQ_RD
        { true,  6 },   // KA
        { true,  7 },   // KB
        { false, 2 }    // Out
    };

    static pthread_t g_monitor_thread;
    static bool g_thread_running = false;
    static bool g_stop_monitoring = false;

    static const int INPUT_PINS_COUNT = sizeof(g_input_pins) / sizeof(g_input_pins[0]);
    static const int OUTPUT_PINS_COUNT = sizeof(g_output_pins) / sizeof(g_output_pins[0]);

    // Вспомогательные функции для получения информации о пинах
    InputPinInfo *GetInputPinInfo(Pin::E type)
    {
        if (type >= Pin::Count) return nullptr;

        auto &mapping = g_pin_mapping[type];
        if (mapping.is_input)
        {
            return &g_input_pins[mapping.index];
        }
        return nullptr;
    }

    OutputPinInfo *GetOutputPinInfo(Pin::E type)
    {
        if (type >= Pin::Count) return nullptr;

        auto &mapping = g_pin_mapping[type];
        if (!mapping.is_input)
        {
            return &g_output_pins[mapping.index];
        }
        return nullptr;
    }

    static void *MonitorThreadFunc(void *arg);
}


namespace GPIO
{
    void Init()
    {
        LOG_WRITE("Initializing GPIO...");

        for (int i = 0; i < INPUT_PINS_COUNT; i++)
        {
            InputPinInfo &info = g_input_pins[i];

            info.hw.chip = gpiod_chip_open_by_name(info.hw.chip_name);
            if (!info.hw.chip)
            {
                LOG_ERROR("Cannot open GPIO chip %s", info.hw.chip_name);
                continue;
            }

            info.hw.line = gpiod_chip_get_line(info.hw.chip, (uint)info.hw.pin_number);
            if (!info.hw.line)
            {
                LOG_ERROR("Cannot get GPIO line %d", info.hw.pin_number);
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
                continue;
            }

            int ret = gpiod_line_request_input_flags(info.hw.line, nullptr,
                GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_DOWN);
            if (ret < 0)
            {
                LOG_ERROR("Cannot request GPIO line %d as input", info.hw.pin_number);
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
                info.hw.line = nullptr;
                continue;
            }

            info.last_state = (gpiod_line_get_value(info.hw.line) == 1);

            LOG_WRITE("GPIO input pin %s:%s:%d initialized", info.hw.name_connector, info.hw.chip_name, info.hw.pin_number);
        }

        for (int i = 0; i < OUTPUT_PINS_COUNT; i++) //-V1008
        {
            OutputPinInfo &info = g_output_pins[i];

            info.hw.chip = gpiod_chip_open_by_name(info.hw.chip_name);
            if (!info.hw.chip)
            {
                LOG_ERROR("Cannot open GPIO chip %s", info.hw.chip_name);
                continue;
            }

            info.hw.line = gpiod_chip_get_line(info.hw.chip, (uint)info.hw.pin_number);
            if (!info.hw.line)
            {
                LOG_ERROR("Cannot get GPIO line for pin %d", info.hw.pin_number);
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
                continue;
            }

            int ret = gpiod_line_request_output(info.hw.line, nullptr, 0);
            if (ret < 0)
            {
                LOG_ERROR("Cannot request GPIO line for pin %d as output", info.hw.pin_number);
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
                info.hw.line = nullptr;
                continue;
            }

            LOG_WRITE("GPIO output pin %s:%s:%d initialized", info.hw.name_connector, info.hw.chip_name, info.hw.pin_number);
        }

        g_stop_monitoring = false;
        if (pthread_create(&g_monitor_thread, nullptr, MonitorThreadFunc, nullptr) == 0)
        {
            g_thread_running = true;
            LOG_WRITE("GPIO monitor thread started");
        }
        else
        {
            LOG_ERROR("Cannot create GPIO monitor thread");
        }
    }

    void DeInit()
    {
        LOG_WRITE("Deinitializing GPIO...");

        if (g_thread_running)
        {
            g_stop_monitoring = true;
            pthread_join(g_monitor_thread, nullptr);
            g_thread_running = false;
        }

        for (int i = 0; i < INPUT_PINS_COUNT; i++)
        {
            InputPinInfo &info = g_input_pins[i];

            if (info.hw.line)
            {
                gpiod_line_release(info.hw.line);
                info.hw.line = nullptr;
            }

            if (info.hw.chip)
            {
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
            }
        }

        for (int i = 0; i < OUTPUT_PINS_COUNT; i++) //-V1008
        {
            OutputPinInfo &info = g_output_pins[i];

            if (info.hw.line)
            {
                gpiod_line_release(info.hw.line);
                info.hw.line = nullptr;
            }

            if (info.hw.chip)
            {
                gpiod_chip_close(info.hw.chip);
                info.hw.chip = nullptr;
            }
        }

        LOG_WRITE("GPIO deinitialized");
    }

    static void *MonitorThreadFunc(void *arg)
    {
        (void)arg;

        LOG_WRITE("GPIO event-driven monitor thread started");

        fd_set read_fds;
        int max_fd = 0;
        int input_pins[INPUT_PINS_COUNT];
        int input_count = 0;

        for (int i = 0; i < INPUT_PINS_COUNT; i++)
        {
            InputPinInfo &info = g_input_pins[i];

            if (!info.hw.line)
                continue;

            gpiod_line_release(info.hw.line);

            int ret = gpiod_line_request_both_edges_events_flags(
                info.hw.line,
                nullptr,
                GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP
            );

            if (ret < 0)
            {
                LOG_ERROR("Cannot request events for GPIO pin %d", info.hw.pin_number);
                continue;
            }

            int fd = gpiod_line_event_get_fd(info.hw.line);
            if (fd < 0)
            {
                LOG_ERROR("Cannot get event fd for GPIO pin %d", info.hw.pin_number);
                continue;
            }

            input_pins[input_count] = i;
            input_count++;

            if (fd > max_fd)
                max_fd = fd;

            info.last_state = (gpiod_line_get_value(info.hw.line) == 1);
        }

        if (input_count == 0)
        {
            LOG_ERROR("No input pins configured for event monitoring");
            return nullptr;
        }

        while (!g_stop_monitoring)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            FD_ZERO(&read_fds);

            for (int i = 0; i < input_count; i++)
            {
                int pin_idx = input_pins[i];
                InputPinInfo &info = g_input_pins[pin_idx];

                if (info.hw.line)
                {
                    int fd = gpiod_line_event_get_fd(info.hw.line);
                    if (fd >= 0)
                    {
#ifdef WIN32
    #pragma warning(push, 0)
#endif
                        FD_SET(fd, &read_fds);
#ifdef WIN32
    #pragma warning(pop)
#endif
                    }
                }
            }

            struct timeval timeout;
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            int result = select(max_fd + 1, &read_fds, nullptr, nullptr, &timeout);

            if (result < 0)
            {
                if (errno == EINTR)
                    continue;

                LOG_ERROR("select() failed in GPIO monitor: %s", strerror(errno));
                break;
            }
            else if (result == 0)
            {
                continue;
            }

            for (int i = 0; i < input_count; i++)
            {
                int pin_idx = input_pins[i];
                InputPinInfo &info = g_input_pins[pin_idx];

                if (!info.hw.line || !info.callback)
                    continue;

                int fd = gpiod_line_event_get_fd(info.hw.line);
                if (fd < 0 || !FD_ISSET(fd, &read_fds))
                    continue;

                struct gpiod_line_event event;
                int ret = gpiod_line_event_read(info.hw.line, &event);

                if (ret < 0)
                {
                    LOG_ERROR("Cannot read GPIO event for pin %d", info.hw.pin_number);
                    continue;
                }

                bool new_state;
                if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE)
                {
                    new_state = true;
                }
                else if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE)
                {
                    new_state = false;
                }
                else
                {
                    continue;
                }

                if (new_state != info.last_state)
                {
                    info.last_state = new_state;

                    info.callback(new_state);

                    LOG_WRITE("GPIO pin %d event: %s -> %s", info.hw.pin_number, new_state ? "RISING" : "FALLING", new_state ? "HIGH" : "LOW");
                }
            }
        }

        LOG_WRITE("GPIO event-driven monitor thread stopped");

        return nullptr;
    }
}


bool PinIn::GetHardware(gpiod_line *line)
{
    return gpiod_line_get_value(line) == 1;
}


bool Pin::Get() const
{
    if (type_ >= Pin::Count) return false;

    // Сначала пробуем как input pin
    InputPinInfo *input_info = GPIO::GetInputPinInfo(type_);
    if (input_info && input_info->hw.line)
    {
        int val = gpiod_line_get_value(input_info->hw.line);
        if (val < 0)
        {
            LOG_ERROR("Error: Cannot read GPIO pin number %d", input_info->hw.pin_number);
            return false;
        }
        return (val == 1);
    }

    // Если не input, то пробуем как output pin
    OutputPinInfo *output_info = GPIO::GetOutputPinInfo(type_);
    if (output_info && output_info->hw.line)
    {
        int val = gpiod_line_get_value(output_info->hw.line);
        if (val < 0)
        {
            LOG_ERROR("Cannot read GPIO pin %d", output_info->hw.pin_number);
            return false;
        }
        return (val == 1);
    }

    return false;
}


void PinOut::Set(bool state)
{
    if (type_ >= Pin::Count) return;

    OutputPinInfo *info = GPIO::GetOutputPinInfo(type_);
    if (!info || !info->hw.line) return;

    int ret = gpiod_line_set_value(info->hw.line, state ? 1 : 0);
    if (ret < 0)
    {
        LOG_ERROR("Error: Cannot set GPIO pin %d to %s", info->hw.pin_number, state ? "HIGH" : "LOW");
    }
}


void PinOut::Set(gpiod_line *line, int state)
{
    gpiod_line_set_value(line, state);
}


void PinOut::ToHi()
{
    Set(true);
}


void PinOut::ToLow()
{
    Set(false);
}


void PinIn::SetChangeCallback(ChangeCallback callback)
{
    callback_ = callback;

    if (type_ < Pin::Count)
    {
        InputPinInfo *info = GPIO::GetInputPinInfo(type_);
        if (info)
        {
            info->callback = callback;
        }
    }
}
