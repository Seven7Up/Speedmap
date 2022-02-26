/*
 * CURSOR.h is C lib which has usefull functions to manipulate cursor and here
 * position.
 */

#ifndef _CURSOR_H
#define _CURSOR_H

struct position {
  int row;
  int col;
};

extern struct termios *settings;
extern int stdout_fileno;
extern struct position win;

struct position *init_position();

int get_pos_report(char *s);

int regfindall_pos(const char *regexp, const char *s, struct position *pos);

struct position *get_current_pos();

struct position *get_terminal_size();

void sync_size();

#endif
