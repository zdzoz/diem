#include "app.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "raw.h"
#include "screen.h"
#include "ui.h"

void init_app(void)
{
    enable_raw_mode();
    init_screen();
}

int run_app(void)
{
    char ch;
    while (1) {
        if (read(STDIN_FILENO, &ch, 1) < 0 && errno != EAGAIN) {
            perror("read");
            return 1;
        }

        ui_draw();

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
