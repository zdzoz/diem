#include "screen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

typedef struct Screen {
    uint16_t width;
    uint16_t height;
    char* buf;
} Screen;

static Screen* screen;
static bool resized = false;

static void disable_cursor(void)
{
    write(STDOUT_FILENO, "\x1b[?25l", 6);
}

static void enable_cursor(void)
{
    write(STDOUT_FILENO, "\x1b[?25h", 6);
}

void clear_screen(void)
{
    memset(screen->buf, ' ', sizeof(char) * screen->width * screen->height);
}

static void get_screen_info(void)
{
    struct winsize window_size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window_size) < 0) {
        perror("ioctl");
        exit(1);
    }

    if (window_size.ws_col != screen->width || window_size.ws_row != screen->height) {
        size_t prev_s = screen->width * screen->height;

        screen->width = window_size.ws_col;
        screen->height = window_size.ws_row;

        size_t s = screen->width * screen->height;
        if (prev_s < s) {
            if (screen->buf != NULL)
                free(screen->buf);
            screen->buf = malloc(sizeof(char) * (s + 1));
            memset(screen->buf, ' ', sizeof(char) * s);
        }
        screen->buf[s] = '\0';
        resized = true;
    }
}

void move_cursor(uint16_t x, uint16_t y)
{
    printf("\x1B[%d;%dH", y, x);
}

void init_screen(void)
{
    screen = malloc(sizeof(Screen));
    memset(screen, 0, sizeof(Screen));
    get_screen_info();
    disable_cursor();
    move_cursor(1, 1);
    clear_screen();
}

void free_screen(void)
{
    enable_cursor();
    free(screen->buf);
    free(screen);
}

void draw(const char* str, uint16_t x, uint16_t y)
{
    int len = strlen(str);
    drawn(str, len, x, y);
}

void drawn(const char* str, int len, uint16_t x, uint16_t y)
{
    if (x + len > screen->width)
        len = (int)screen->width - (int)x;
    if (y >= screen->height || len <= 0)
        return;
    memcpy(screen->buf + 1 * (screen->width * y + x), str, sizeof(char) * len);
}

void draw_screen(void)
{
    get_screen_info();

    // static int i = 0;
    // memset(screen->buf + screen->width * i, '#', sizeof(char) * screen->width);
    // i = (i + 1) % (screen->height);
    printf("\x1B[1;1H%s", screen->buf);
    fflush(stdout);
    clear_screen();
}

char* get_screen(void)
{
    return screen->buf;
}

uint16_t screen_height(void)
{
    return screen->height;
}

uint16_t screen_width(void)
{
    return screen->width;
}

bool screen_resized(void)
{
    bool r = resized;
    resized = false;
    return r;
}
