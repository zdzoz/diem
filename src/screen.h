#ifndef _H_SCREEN
#define _H_SCREEN

#include <stdint.h>

void init_screen(void);
void draw_screen(void);
void free_screen(void);


void draw(const char* str, uint16_t x, uint16_t y);

uint16_t screen_height(void);
uint16_t screen_width(void);

void move_cursor(uint16_t x, uint16_t y);

#endif // _H_SCREEN
