#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "cursor.h"
#include "log.h"

int main() {
  signal(SIGWINCH, sync_size);
  struct position *pos = get_current_pos();

  sync_size();

  char *str_list[] = {"Hello", "World", "!"};
  int index = 0;
  while (NULL != str_list[index]) {
    info("str_list[%d]='%s'", index, str_list[index]);
    index++;
  }

  return 0;
}
