#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>

#include "log.h"

const colors colors_codes = {
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
    char *string = (char *)malloc(8);
    snprintf(string, 8, "\x1b[%dm", code);
    return string;
};

// Source: https://www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/
char *replace_word(const char* s, const char* oldW, const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
  
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
  
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
  
    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
  
    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
  
    result[i] = '\0';
    return result;
}

int info(char *string, ...) {
    char *code_str = get_color_str(colors_codes.BLUE);
    char *reset_str = get_color_str(colors_codes.RESET);
    char *formatted_str = replace_word(string, "\n", "\n    ");

    va_list va_args;
    va_start(va_args, string);
    printf("[%s*%s] ", code_str, reset_str);
    vprintf(formatted_str, va_args);
    printf("\n");
    va_end(va_args);

    free(code_str);
    free(reset_str);
    return 0;
};

int succ(char *string, ...) {
    char *code_str = get_color_str(colors_codes.GREEN);
    char *reset_str = get_color_str(colors_codes.RESET);
    char *formatted_str = replace_word(string, "\n", "\n    ");

    va_list va_args;
    va_start(va_args, string);
    printf("[%s+%s] ", code_str, reset_str);
    vprintf(formatted_str, va_args);
    printf("\n");
    va_end(va_args);
    
    free(code_str);
    free(reset_str);
    return 0;
};

int err(char *string, ...) {
    char *code_str = get_color_str(colors_codes.RED);
    char *reset_str = get_color_str(colors_codes.RESET);
    char *formatted_str = replace_word(string, "\n", "\n    ");
    
    va_list va_args;
    va_start(va_args, string);
    printf("[%s-%s] ", code_str, reset_str);
    vprintf(formatted_str, va_args);
    printf("\n");
    va_end(va_args);
    
    free(code_str);
    free(reset_str);
    return 0;
};

void err_exit(char *msg, ...) {
    va_list va_args;
    va_start(va_args, msg);
    printf("[%s-%s] ", get_color_str(colors_codes.RED), get_color_str(colors_codes.RESET)); // Program will exit, so no need to free any things
    vprintf(msg, va_args);
    printf("\n");
    va_end(va_args);
    exit(1);
};

int warn(char *string, ...) {
    char *code_str = get_color_str(colors_codes.YELLOW);
    char *reset_str = get_color_str(colors_codes.RESET);
    char *formatted_str = replace_word(string, "\n", "\n    ");
    
    va_list va_args;
    va_start(va_args, string);
    printf("[%s!%s] ", code_str, reset_str);
    vprintf(formatted_str, va_args);
    printf("\n");
    va_end(va_args);
    
    free(code_str);
    free(reset_str);
    return 0;
};

int allow_debug = 0; // false
int debug(char *string, ...) {
    if (allow_debug) { // to enable it, you need to change allow_debug to 1 wich is true
        char *code_str = get_color_str(colors_codes.MAGENTA);
        char *reset_str = get_color_str(colors_codes.RESET);
        char *formatted_str = replace_word(string, "\n", "\n        ");

        va_list va_args;
        va_start(va_args, string);
        printf("[%sDEBUG%s] ", code_str, reset_str);
        vprintf(formatted_str, va_args);
        // printf("\n");
        va_end(va_args);

        free(code_str);
        free(reset_str);
        return 0;
    }
    return 1;
};