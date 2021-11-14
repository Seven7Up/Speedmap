#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>

#ifndef LOG_H
#define LOG_H

typedef struct {
const int
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    RESET,
    LIGHTBLACK_EX,
    LIGHTRED_EX,
    LIGHTGREEN_EX,
    LIGHTYELLOW_EX,
    LIGHTBLUE_EX,
    LIGHTMAGENTA_EX,
    LIGHTCYAN_EX,
    LIGHTWHITE_EX;
} colors;


char *get_color_str(const int code);

// Source: https://www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/
char *replace_word(const char* s, const char* oldW, const char* newW);

int info(char *string, ...);

int succ(char *string, ...);

int err(char *string, ...);

int debug(char *string, ...);

int warn(char *string, ...);

#endif // LOG_H