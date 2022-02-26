#ifndef __LIST_H
#define __LIST_H

typedef struct node_t {
  char *value;
  struct node_t *next;
} node_t;

node_t *init_list_lst();

const node_t *init_parent_lst();

node_t *get_child_lst(const node_t *parent, const int index);

int set_value_lst(const node_t *parent, const int index, const char *value);

int get_length_lst(const node_t *parent);

int add_child_lst(const node_t *parent, const char *value);

int free_all_lst(node_t **parent);

int free_item_lst(const node_t *parent, const int index);

int delete_child_lst(node_t **parent, const int index);

#endif
