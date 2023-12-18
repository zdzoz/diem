#include "ui.h"

#include "screen.h"
#include <string.h>

typedef struct container {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    char* title;
} container_t;

typedef struct container_opts {
    struct {
        bool top;
        bool bot;
        bool left;
        bool right;
    } border;
} container_opts_t;

#define CREATE_CONTAINER(c, x, y, w, h, t) \
    container_t c = { .x = x, .y = y, .width = w - 2, .height = h - 2, .title = t }

#define UI_CATEGORIES_MIN_WIDTH 25
#define UI_CATEGORIES_MAX_WIDTH 50
#define UI_CATEGORIES_MIN_HEIGHT 25
container_t ui_categories_container = { .x = 1, .y = 1, .width = 0, .height = 0, .title = "Categories" };
container_t ui_task_container = { .x = 0, .y = 0, .width = 0, .height = 0, .title = "Tasks" };

typedef struct border_char {
    char tl;
    char tr;
    char bl;
    char br;
    char h;
    char v;
} border_char_t;

static const border_char_t border_chars = { '+', '+', '+', '+', '-', '|' };

#define UI_DRAW_CONTAINER(c) ui_draw_container(c, NULL)

// TODO: implement support for utf-8
static void ui_draw_container(container_t* c, container_opts_t* opts)
{
    (void)opts;
    char buf[255] = { 0 };

    memset(buf, border_chars.h, sizeof(char) * c->width + 1);
    if (!opts || (opts && opts->border.top)) {
        draw(buf, c->x - 1, c->y - 1); // top
        if (c->title)
            drawn(c->title, strlen(c->title), c->x + 2, c->y - 1);
    }

    if (!opts || (opts && opts->border.bot))
        draw(buf, c->x - 1, c->height + 1); // bottom

    for (uint16_t i = c->y - 1; i < c->height + 1; i++) {
        if (!opts || (opts && opts->border.left))
            drawn(&border_chars.v, 1, c->x - 1, i);
        if (!opts || (opts && opts->border.right))
            drawn(&border_chars.v, 1, c->width + 1, i);
    }

    drawn(&border_chars.tl, 1, c->x - 1, c->y - 1);
    drawn(&border_chars.tr, 1, c->width + 1, c->y - 1);
    drawn(&border_chars.bl, 1, c->x - 1, c->height + 1);
    drawn(&border_chars.br, 1, c->width + 1, c->height + 1);
}

static void update(void)
{
    // update ui categories
    uint16_t width = screen_width() / 4;
    uint16_t height = screen_height();
    width = width < UI_CATEGORIES_MIN_WIDTH ? UI_CATEGORIES_MIN_WIDTH : width;
    width = width > UI_CATEGORIES_MAX_WIDTH ? UI_CATEGORIES_MAX_WIDTH : width;
    height = height < UI_CATEGORIES_MIN_HEIGHT ? UI_CATEGORIES_MIN_HEIGHT : height;

    ui_categories_container.width = width;
    ui_categories_container.height = height;

    // update ui tasks
    ui_task_container.x = ui_categories_container.x + ui_categories_container.width - 1;
    ui_task_container.y = ui_categories_container.y;
    ui_task_container.width = screen_width();
    ui_task_container.height = ui_categories_container.height;
}

void drawc(container_t* c, const char* str, uint16_t x, uint16_t y)
{
    int len = strlen(str);
    int _x = c->x + x;
    int _y = c->y + y;

    if (_x + len > c->width + 1)
        len = (int)c->width + 1 - _x;
    if (_y >= c->height + 1 || len <= 0)
        return;
    drawn(str, len, _x, _y);
}

void ui_draw(void)
{
    if (screen_resized())
        update();

    // UI_DRAW_CONTAINER(&ui_categories_container);
    // UI_DRAW_CONTAINER(&ui_task_container);

    // char separator[UI_CATEGORIES_MAX_WIDTH] = { 0 };
    // memset(separator, '-', sizeof(char) * ui_categories_container.width - 1);
    // drawc(&ui_categories_container, "| test |", 7, 1);
    // drawc(&ui_categories_container, separator, 0, 2);

    container_t tcont = { .x = 1, .y = 1, .width = screen_width() - 2, .height = screen_height() - 2, .title = "TEST" };
    UI_DRAW_CONTAINER(&tcont);
    drawc(&tcont, "test", 0, 0);
    drawc(&tcont, "test", tcont.width - 2, 0);
    drawc(&tcont, "test", 0, tcont.height - 1);
    drawc(&tcont, "test", tcont.width - 1, tcont.height - 1);

    draw_screen();
}
