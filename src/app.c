#include "app.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "raw.h"
#include "screen.h"

void init_app(void)
{
    enable_raw_mode();
    init_screen();
}

int run_app(void)
{
    char ch;
    const char* welcome = "Welcome to Diem";
    int welcome_len = strlen(welcome);
    while (1) {
        if (read(STDIN_FILENO, &ch, 1) < 0 && errno != EAGAIN) {
            perror("read");
            return 1;
        }


        draw(welcome, screen_width() / 2 - (welcome_len / 2), screen_height() / 2);

        draw_screen();

        if (ch == 'q')
            break;
    }

    return 0;
}

void free_app(void)
{
    disable_raw_mode();
    free_screen();
    move_cursor(1, 1);
}
