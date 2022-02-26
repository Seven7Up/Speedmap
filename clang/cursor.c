/*
 * CURSOR.c is the functions dictionary for cursor.h
 */

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "cursor.h"

struct termios *settings;
int stdout_fileno;
struct position win;

struct position *init_position() {
  return malloc(sizeof(struct position));
}

int setup() {
  struct termios mode;

  if (tcgetattr(stdout_fileno, &mode)) {
    fprintf(stderr, "Error: tcgetattr()\n");
    return 1;
  }

  mode.c_lflag &= ~(ECHO | ICANON | IEXTEN);

  if (tcsetattr(stdout_fileno, TCSAFLUSH, &mode)) {
    fprintf(stderr, "Error: tcsetattr()\n");
    return 1;
  }
  return 0;
}

int reset() {
  if (tcsetattr(stdout_fileno, TCSADRAIN, settings)) {
    fprintf(stderr, "Error: tcsetattr()\n");
    return 1;
  }
  return 0;
}

int get_pos_report(char *s) {

  settings = (struct termios *)malloc(sizeof(struct termios));
  stdout_fileno = fileno(stdout);

  if (tcgetattr(stdout_fileno, settings)) {
    fprintf(stderr, "Error: tcgetattr()\n");
    return 1;
  }

  char c;
  int i;

  setup();

  fprintf(stdout, "\x1b[6n");
  fflush(stdout);

  i = 0;
  while ((c = fgetc(stdin)) != EOF) {
    *(s + i) = c;
    if (c == 'R') {
      *(s + i + 1) = 0;
      break;
    }
    i++;
  }

  reset();
  return 0;
}

int regfindall_pos(const char *regexp, const char *s, struct position *pos) {
  char *p = (char *)s;
  regex_t regex;
  int status;

  status = regcomp(&regex, regexp, REG_EXTENDED | REG_NEWLINE);
  if (status) {
    char error_msg[100];
    regerror(status, &regex, error_msg, 100);
    fprintf(stderr, "Error: %s", error_msg);
    return status;
  }

  unsigned nmatches = 16;
  regmatch_t matches[nmatches];

  while (1) {

    status = regexec(&regex, p, nmatches, matches, 0);
    if (status && p == s) {
      char error_msg[100];
      regerror(status, &regex, error_msg, 100);
      fprintf(stderr, "Error: %s", error_msg);
      return status;
    } else if (status) {
      /* printf("No more matches!\n"); */
      return 0;
    }

    for (int i = 0; i <= nmatches; i++) {
      if (matches[i].rm_so == -1)
        break;

      int length = matches[i].rm_eo - matches[i].rm_so;
      char match[length];

      strncpy(match, p + matches[i].rm_so, length);
      match[length] = 0;

      /* printf("str[%i:%i] = '%s';\n", matches[i].rm_so, matches[i].rm_eo,
       * match); */

      if (i == 1)
        pos->row = atoi(match);
      else if (i == 2)
        pos->col = atoi(match);
    }
    p += matches[0].rm_eo;
  }

  regfree(&regex);
  return 0;
}

struct position *get_current_pos() {
  char *s = (char *)malloc(32);
  char regexp[64] = "\x1b\\[([[:digit:]]+);([[:digit:]]+)R";
  struct position *pos = init_position();

  get_pos_report(s);
  regfindall_pos(regexp, s, pos);

  free(s);
  return pos;
}

struct position *get_terminal_size() {
  struct position *pos = init_position();
  struct winsize w;

  ioctl(stdout_fileno, TIOCGWINSZ, &w);
  pos->row = w.ws_row;
  pos->col = w.ws_col;

  return pos;
}

void sync_size() { win = *get_terminal_size(); }
