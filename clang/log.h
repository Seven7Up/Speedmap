/*
 * My best, the LOG.h header defines logging levels and functions.
 */

#ifndef _LOGGER
#define _LOGGER

#include "ansi_escapes.h"
#include "cursor.h"

typedef struct {
  const char *name;
  const int code;
  const char *prefix;
  const int indent;
  const enum colors color;
} _log_level;

struct progress {
  char *name;
  int status;
  struct position pos;
};

extern const _log_level noset_level;
extern const _log_level critical_level;
extern const _log_level warning_level;
extern const _log_level success_level;
extern const _log_level error_level;
extern const _log_level info_level;
extern const _log_level progress_level;
extern const _log_level debug_level;

extern _log_level *global_log_level;

int set_level(const _log_level *level);
char *get_level();

int noset(const char *str, ...);
int critical(const char *str, ...);
int warning(const char *str, ...);
int success(const char *str, ...);
int error(const char *str, ...);
int info(const char *str, ...);
int debug(const char *str, ...);

#endif
