#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cursor.h"
#include "list.h"
#include "log.h"

int main(int argc, char *argv[]) {
  struct position *pos = malloc(sizeof(struct position));
  node_t *parent = malloc(sizeof(node_t));

  pos = get_current_pos_cur();
  info("row=%d, col=%d", pos->row, pos->col);

  free(pos);
  return 0;
}
