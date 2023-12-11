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

static void disable_cursor(void)
{
    write(STDOUT_FILENO, "\x1b[?25l", 6);
}

static void enable_cursor(void)
{
    write(STDOUT_FILENO, "\x1b[?25h", 6);
}

static void clear_screen(void)
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
    }
}

void move_cursor(uint16_t x, uint16_t y)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "\x1b[%d;%dH", y, x);
    write(STDOUT_FILENO, buffer, strlen(buffer));
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
    size_t len = strlen(str);
    if (x + len > screen->width)
        len = screen->width - x;
    if (y > screen->height)
        return;
    memcpy(screen->buf + screen->width * y + x, str, sizeof(char) * len);
}

static __unused void draw_border(void) {
    // TODO: utf-8 support
    // char border_chars[] = {'┌', '┐', '└', '┘', '─' ,'│'};
    char border_chars[] = {'+', '+', '+', '+', '-', '|'}; // tl, tr, bl, br, h, v

    memset(screen->buf, border_chars[4], sizeof(char) * screen->width);
    memset(screen->buf + screen->width * (screen->height - 1), border_chars[4], sizeof(char) * screen->width);
    for (int i = 1; i < screen->height - 1; i++) {
        screen->buf[screen->width * i] = border_chars[5];
        screen->buf[screen->width * i + screen->width - 1] = border_chars[5];
    }
    screen->buf[0] = border_chars[0];
    screen->buf[screen->width - 1] = border_chars[1];
    screen->buf[screen->width * (screen->height - 1)] = border_chars[2];
    screen->buf[screen->width * screen->height - 1] = border_chars[3];
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

uint16_t screen_height(void) {
    return screen->height;
}

uint16_t screen_width(void) {
    return screen->width;
}
