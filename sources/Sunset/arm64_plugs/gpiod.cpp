// 2026/05/04 10:36:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "gpiod.h"
#include <map>


#ifndef ARM64

int gpiod_line_event_get_fd(struct gpiod_line * /*line*/)
{
    return 0;
}

struct gpiod_chip *gpiod_chip_open_by_name(const char *name)
{
    static std::map<pchar, gpiod_chip> chips;

    if (chips.find(name) == chips.end())
    {
        gpiod_chip chip{ (int)chips.size() };

        chips[name] = chip;
    }

    return &chips.find(name)->second; //-V783
}

struct gpiod_line *gpiod_chip_get_line(struct gpiod_chip *chip, unsigned int /*offset*/)
{
    static std::map<gpiod_chip *, gpiod_line> lines;

    if (lines.find(chip) == lines.end())
    {
        gpiod_line line{ (int)lines.size() };

        lines[chip] = line;
    }

    return &lines.find(chip)->second; //-V783
}

void gpiod_chip_close(struct gpiod_chip * /*chip*/)
{
}

int gpiod_line_request_output(struct gpiod_line * /*line*/, const char * /*consumer*/, int /*default_val*/)
{
    return 0;
}

int gpiod_line_request_input_flags(struct gpiod_line * /*line*/, const char * /*consumer*/, unsigned int /*flags*/)
{
    return 0;
}

int gpiod_line_request_both_edges_events_flags(struct gpiod_line * /*line*/, const char * /*consumer*/, unsigned int /*flags*/)
{
    return 0;
}

int gpiod_line_get_value(struct gpiod_line * /*line*/)
{
    return 0;
}

void gpiod_line_release(struct gpiod_line * /*line*/)
{
}

int gpiod_line_event_read(gpiod_line *, struct gpiod_line_event * /*event*/)
{
    return 0;
}

int gpiod_line_set_value(struct gpiod_line * /*line*/, int /*value*/)
{
    return 0;
}

#endif
