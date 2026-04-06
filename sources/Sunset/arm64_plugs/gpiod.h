#pragma once


#ifndef ARM64

#define GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP     (1 << 1)
#define GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_DOWN   (1 << 1)

struct gpiod_chip
{
    int num;
};


struct gpiod_line
{
    int num;
};


enum gpiod_line_event_type {
    GPIOD_LINE_EVENT_RISING_EDGE = 1,  // Передний фронт (0 → 1)
    GPIOD_LINE_EVENT_FALLING_EDGE = 2, // Задний фронт (1 → 0)
};

struct gpiod_line_event {
    struct timespec ts;      // Время возникновения события
    enum gpiod_line_event_type event_type;  // Тип события
};

int gpiod_line_event_get_fd(struct gpiod_line *line);
struct gpiod_chip *gpiod_chip_open_by_name(const char *name);
struct gpiod_line *gpiod_chip_get_line(struct gpiod_chip *chip, unsigned int offset);
void gpiod_chip_close(struct gpiod_chip *chip);
int gpiod_line_request_output(struct gpiod_line *line, const char *consumer, int default_val);
int gpiod_line_request_input_flags(struct gpiod_line *line, const char *consumer, unsigned int flags);
int gpiod_line_request_both_edges_events_flags(struct gpiod_line *line, const char *consumer, unsigned int flags);
int gpiod_line_get_value(struct gpiod_line *line);
void gpiod_line_release(struct gpiod_line *line);
int gpiod_line_event_read(gpiod_line *, struct gpiod_line_event *event);
int gpiod_line_set_value(struct gpiod_line *line, int value);

#endif
