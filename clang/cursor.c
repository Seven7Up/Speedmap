/*
 * CURSOR.c is the functions dictionary for cursor.h
 */

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "cursor.h"
#include "list.h"

struct termios *settings;
int stdout_fileno;

int setup_cur() {
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

int reset_cur() {
  if (tcsetattr(stdout_fileno, TCSADRAIN, settings)) {
    fprintf(stderr, "Error: tcsetattr()\n");
    return 1;
  }
  return 0;
}

int get_pos_report_cur(char *s) {

  settings = (struct termios *)malloc(sizeof(struct termios));
  stdout_fileno = fileno(stdout);

  if (tcgetattr(stdout_fileno, settings)) {
    fprintf(stderr, "Error: tcgetattr()\n");
    return 1;
  }

  char c;
  int i;

  setup_cur();

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

  reset_cur();
  return 0;
}

int reg_findall_cur(const char *regexp, const char *s, const node_t *parent) {
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

      if (i != 0)
        add_child_lst(parent, strdup(match));
    }
    p += matches[0].rm_eo;
  }

  regfree(&regex);
  return 0;
}

int list2pos_cur(const node_t *parent, struct position *pos) {
  int length;
  node_t *ptr;

  length = get_length_lst(parent);
  for (int i = 0; i < length; i++) {
    ptr = get_child_lst(parent, i);

    if (ptr->value == NULL)
      continue;
    if (!i)
      pos->row = atoi(ptr->value);
    else if (i == 1)
      pos->col = atoi(ptr->value);
    else
      return 1;
  }

  return 0;
}

struct position *get_current_pos_cur() {
  char *s = (char *)malloc(32);
  char regexp[64] = "\x1b\\[([[:digit:]]+);([[:digit:]]+)R";
  const node_t *parent = init_parent_lst();
  struct position *pos = malloc(sizeof(struct position));

  get_pos_report_cur(s);
  reg_findall_cur(regexp, s, parent);
  list2pos_cur(parent, pos);

  free(s);
  free_all_lst((node_t **)&parent);
  return pos;
}
