#include <stdio.h>
#include <malloc.h>

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

colors colors_codes = {
    .BLACK = 30,
    .RED = 31,
    .GREEN = 32,
    .YELLOW = 33,
    .BLUE = 34,
    .MAGENTA = 35,
    .CYAN = 36,
    .WHITE = 37,
    .RESET = 39,
    .LIGHTBLACK_EX = 90,
    .LIGHTRED_EX = 91,
    .LIGHTGREEN_EX = 92,
    .LIGHTYELLOW_EX = 93,
    .LIGHTBLUE_EX = 94,
    .LIGHTMAGENTA_EX = 95,
    .LIGHTCYAN_EX = 96,
    .LIGHTWHITE_EX = 97,
};

char *get_color_str(const int code) {
    char *string = malloc(8);
    snprintf(string, 8, "\x1b[%dm", code);
    return string;
};

int info(char *string) {
    char *code_str = get_color_str(colors_codes.BLUE);
    char *reset_str = get_color_str(colors_codes.RESET);
    printf("[%s*%s] %s\n", code_str, reset_str, string);
    free(code_str);
    free(reset_str);
    return 0;
};

int succ(char *string) {
    char *code_str = get_color_str(colors_codes.GREEN);
    char *reset_str = get_color_str(colors_codes.RESET);
    printf("[%s*%s] %s\n", code_str, reset_str, string);
    free(code_str);
    free(reset_str);
    return 0;
};

int err(char *string) {
    char *code_str = get_color_str(colors_codes.RED);
    char *reset_str = get_color_str(colors_codes.RESET);
    printf("[%s*%s] %s\n", code_str, reset_str, string);
    free(code_str);
    free(reset_str);
    return 0;
};

int debug(char *string) {
    char *code_str = get_color_str(colors_codes.MAGENTA);
    char *reset_str = get_color_str(colors_codes.RESET);
    printf("%sDEBUG%s: %s\n", code_str, reset_str, string);
    free(code_str);
    free(reset_str);
    return 0;
};

int warn(char *string) {
    char *code_str = get_color_str(colors_codes.YELLOW);
    char *reset_str = get_color_str(colors_codes.RESET);
    printf("%sWARNING%s: %s\n", code_str, reset_str, string);
    free(code_str);
    free(reset_str);
    return 0;
};