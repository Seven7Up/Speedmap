/*
 * LOG.c is where functions are declared and there definitions.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ansi_escapes.h"
#include "log.h"

const _log_level noset_level = {"noset", 0, "", 0, reset_fore};
const _log_level critical_level = {"critical", 1, "CRITICAL", 11, red_back};
const _log_level warning_level = {"warning", 2, "WARNING", 10, yellow_fore};
const _log_level success_level = {"success", 3, "+", 4, green_fore};
const _log_level error_level = {"error", 3, "-", 4, red_fore};
const _log_level info_level = {"info", 4, "*", 4, blue_fore};
const _log_level debug_level = {"debug", 5, "DEBUG", 8, magenta_fore};

_log_level *global_log_level = (_log_level *)&info_level;

int set_level(const _log_level *level) {
  global_log_level = (_log_level *)level;
  if (global_log_level->code != level->code)
    return 1;
  return 0;
}

char *get_level() {
  char *current_level_name = (char *)global_log_level->name;
  return strdup(current_level_name);
}

// Source:
// https://stackoverflow.com/questions/779875/what-function-is-to-replace-a-substring-from-a-string-in-c
const char *str_replace(char *orig, char *rep, char *with) {
  char *result;
  char *ins;
  char *tmp;
  int len_rep;
  int len_with;
  int len_front;
  int count;
  if (!orig || !rep)
    return NULL;
  len_rep = strlen(rep);
  if (len_rep == 0)
    return NULL;
  if (!with)
    with = "";
  len_with = strlen(with);

  ins = orig;
  for (count = 0; tmp = strstr(ins, rep); ++count) {
    ins = tmp + len_rep;
  }

  tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

  if (!result)
    return NULL;

  while (count--) {
    ins = strstr(orig, rep);
    len_front = ins - orig;
    tmp = strncpy(tmp, orig, len_front) + len_front;
    tmp = strcpy(tmp, with) + len_with;
    orig += len_front + len_rep;
  }
  strcpy(tmp, orig);
  return result;
}

int _log(const _log_level *level, const char *str, va_list args) {
  if (level->code > global_log_level->code) {
    return 1;
  }

  char prefix[64];
  char *color;
  char *rtn_color;
  char join_str_arg[64];
  char *joined_str;
  char *formatted_str;
  char *tmp = (char *)str;
  int ret;

  strcat(join_str_arg, "\n");
  for (int i = 0; i <= level->indent; i++)
    strcat(join_str_arg, " ");

  color = code2str_ans(level->color);
  rtn_color = code2str_ans(reset_color);
  sprintf(prefix, "[%s%s%s] ", color, level->prefix, rtn_color);

  joined_str = str_replace(tmp, "\n", join_str_arg);
  if (!joined_str)
    return 1;

  formatted_str = (char *)malloc(1024);
  strcat(formatted_str, prefix);
  strcat(formatted_str, joined_str);
  strcat(formatted_str, "\n");
  strcat(formatted_str, "\x00");

  ret = vprintf(formatted_str, args);

  free(color);
  free(rtn_color);
  free(joined_str);
  free(formatted_str);
  return ret;
}

int noset(const char *str, ...) {
  int ret;
  va_list args;

  va_start(args, str);
  ret = _log(&noset_level, str, args);
  va_end(args);

  return ret;
}

int critical(const char *str, ...) {
  int ret;
  va_list args;

  va_start(args, str);
  ret = _log(&critical_level, str, args);
  va_end(args);

  return ret;
}

int warning(const char *str, ...) {
  int ret;
  va_list args;

  va_start(args, str);
  ret = _log(&warning_level, str, args);
  va_end(args);

  return ret;
}

int success(const char *str, ...) {
  int ret;
  va_list args;

  va_start(args, str);
  ret = _log(&success_level, str, args);
  va_end(args);

  return ret;
}

int error(const char *str, ...) {
  int ret;
  va_list args;

  va_start(args, str);
  ret = _log(&error_level, str, args);
  va_end(args);

  return ret;
}

int info(const char *str, ...) {
  int ret;
  va_list args;

  va_start(args, str);
  ret = _log(&info_level, str, args);
  va_end(args);

  return ret;
}

int debug(const char *str, ...) {
  int ret;
  va_list args;

  va_start(args, str);
  ret = _log(&debug_level, str, args);
  va_end(args);

  return ret;
}
