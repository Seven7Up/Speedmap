/*
 * CURSOR.h is C lib which has usefull functions to manipulate cursor and here
 * position.
 */

#ifndef _CURSOR_H
#define _CURSOR_H

#include "list.h"

struct position {
  int row;
  int col;
};

extern struct termios *settings;
extern int stdout_fileno;

int setup_console_cur();

int reset_console_cur();

int get_pos_report_cur(char *s);

int reg_findall_cur(const char *regexp, const char *s, const node_t *parent);

int list2pos_cur(const node_t *parent, struct position *pos);

struct position *get_current_pos_cur();

#endif
