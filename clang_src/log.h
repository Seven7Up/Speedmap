#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// Source: https://www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/
char* replace_word(const char* s, const char* oldW,
                  const char* newW)
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

    va_list comp;
    va_start(comp, string);
    printf("[%s*%s] %s\n", code_str, reset_str, formatted_str, comp);
    va_end(comp);

    free(code_str);
    free(reset_str);
    return 0;
};

int succ(char *string, ...) {
    char *code_str = get_color_str(colors_codes.GREEN);
    char *reset_str = get_color_str(colors_codes.RESET);
    char *formatted_str = replace_word(string, "\n", "\n    ");

    va_list comp;
    va_start(comp, string);
    printf("[%s+%s] %s\n", code_str, reset_str, formatted_str, comp);
    va_end(comp);
    
    free(code_str);
    free(reset_str);
    return 0;
};

int err(char *string, ...) {
    char *code_str = get_color_str(colors_codes.RED);
    char *reset_str = get_color_str(colors_codes.RESET);
    char *formatted_str = replace_word(string, "\n", "\n    ");
    
    va_list comp;
    va_start(comp, string);
    printf("[%s-%s] %s\n", code_str, reset_str, formatted_str, comp);
    va_end(comp);
    
    free(code_str);
    free(reset_str);
    return 0;
};

int debug(char *string, ...) {
    char *code_str = get_color_str(colors_codes.MAGENTA);
    char *reset_str = get_color_str(colors_codes.RESET);
    char *formatted_str = replace_word(string, "\n", "\n         ");
    
    va_list comp;
    va_start(comp, string);
    printf("[%sDEBUG%s]: %s\n", code_str, reset_str, formatted_str, comp);
    va_end(comp);
    
    free(code_str);
    free(reset_str);
    return 0;
};

int warn(char *string, ...) {
    char *code_str = get_color_str(colors_codes.YELLOW);
    char *reset_str = get_color_str(colors_codes.RESET);
    char *formatted_str = replace_word(string, "\n", "\n          ");
    
    va_list comp;
    va_start(comp, string);
    printf("[%sWARNING%s]: %s\n", code_str, reset_str, formatted_str, comp);
    va_end(comp);
    
    free(code_str);
    free(reset_str);
    return 0;
};