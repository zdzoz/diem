// Created by: Oskar Zdziarski 2023

#include "raw.h"

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios original_termios;

static void die(const char* s)
{
    perror(s);
    exit(1);
}

void enable_raw_mode(void)
{
    if (tcgetattr(STDIN_FILENO, &original_termios) < 0) {
        die("tcgetattr");
    }

    atexit(disable_raw_mode);

    struct termios raw;
    raw = original_termios;

    raw.c_iflag &= ~(
        IXON | // Disable Ctrl-S and Ctrl-Q
        BRKINT | // Disable Ctrl-C
        INPCK | // Disable parity checking
        ISTRIP | // Disable stripping of 8th bit
        ICRNL // Disable Ctrl-M
    );

    raw.c_oflag &= ~(
        OPOST // Disable output processing
    );

    raw.c_cflag |= (CS8); // Set character size to 8 bits per byte

    raw.c_lflag &= ~(
        ECHO | // Disable echoing of input
        ICANON | // Disable canonical mode
        IEXTEN | // Disable Ctrl-V (Ctrl-O on macOS)
        ISIG // Disable Ctrl-C and Ctrl-Z (SIGINT and SIGTSTP)
    );
    raw.c_cc[VMIN] = 0; // Set minimum number of bytes of input needed before read() can return
    raw.c_cc[VTIME] = 1; // Set maximum amount of time to wait before read() returns

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) < 0) {
        die("tcsetattr");
    }
}

void disable_raw_mode(void)
{
    write(STDOUT_FILENO, "\x1b[?25h", 6);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) < 0) {
        die("tcsetattr");
    }
}
