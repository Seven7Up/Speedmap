#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "ansi_escapes.h"

#ifdef _WIN32
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

static HANDLE stdout_handle;
static DWORD out_mode_init;

void setup_console_ansi(void) {
  DWORD out_mode = 0;
  stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

  if (stdout_handle == INVALID_HANDLE_VALUE) {
    exit(GetLastError());
  }

  if (!GetConsoleMode(stdout_handle, &out_mode)) {
    exit(GetLastError());
  }

  out_mode_init = out_mode;

  out_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

  if (!SetConsoleMode(stdout_handle, out_mode)) {
    exit(GetLastError());
  }
}

void restore_console_ansi(void) {
  char *restore_color = code2str_ans(reset_color);
  if (!restore_color)
    exit(GetLastError());
  printf("%s", restore_color);
  free(restore_color);

  if (!SetConsoleMode(stdout_handle, out_mode_init)) {
    exit(GetLastError());
  }
}
#else
void setup_console_ansi(void) {}

void restore_console_ansi(void) {
  char *restore_color = ansicode2str(reset_color);
  if (!restore_color)
    return;
  printf("%s", restore_color);
  free(restore_color);
}
#endif

char *ansicode2str(const int color_code) {
  char *color_code_string = (char *)malloc(7);
  sprintf(color_code_string, "\x1b[%dm", color_code);
  return color_code_string;
}
