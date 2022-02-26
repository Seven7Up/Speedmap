/*
 * ANSI_ESCAPES.h is a C header which define some essential functions to
 * manupilate any OS console.
 */

#ifndef _ANSI_ESCAPES_H
#define _ANSI_ESCAPES_H

void setup_console_ans(void);
void restore_console_ans(void);

enum colors {
  reset_color,

  black_fore = 30,
  red_fore,
  green_fore,
  yellow_fore,
  blue_fore,
  magenta_fore,
  cyan_fore,
  white_fore,
  reset_fore = 39,

  black_back = 40,
  red_back,
  green_back,
  yellow_back,
  blue_back,
  magenta_back,
  cyan_back,
  white_back,
  reset_back = 49
};

char *code2str_ans(const int color_code);

#endif
