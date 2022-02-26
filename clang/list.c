#include <stdio.h>
#include <stdlib.h>

#include "list.h"

node_t *init_list_lst() { return malloc(sizeof(node_t)); }

const node_t *init_parent_lst() { return (const node_t *)init_list_lst(); }

node_t *get_child_lst(const node_t *parent, const int index) {
  node_t *ptr;

  ptr = (node_t *)parent;
  for (int i = 0; i < index; i++) {
    if (ptr->next == NULL)
      ptr->next = init_list_lst();
    ptr = ptr->next;
  }

  return ptr;
}

int set_value_lst(const node_t *parent, const int index, const char *value) {
  node_t *ptr;

  ptr = get_child_lst(parent, index);
  ptr->value = (char *)value;

  return 0;
}

int get_length_lst(const node_t *parent) {
  int length = 1;
  node_t *ptr;

  ptr = (node_t *)parent;
  while (ptr->next != NULL) {
    ptr = ptr->next;
    length++;
  }

  return length;
}

int add_child_lst(const node_t *parent, const char *value) {
  node_t *ptr;
  int index;

  index = get_length_lst(parent) - 1;
  ptr = get_child_lst(parent, index);
  if (!index && ptr->value != NULL) {
    ptr = get_child_lst(parent, index + 1);
  }

  ptr->value = (char *)value;

  return 0;
}

int free_all_lst(node_t **parent) {
  node_t *ptr;

  while ((*parent)->next != NULL) {
    ptr = *parent;
    while (ptr->next->next != NULL) {
      ptr = ptr->next;
    }
    free(ptr->value);
    free(ptr->next);
    ptr->next = NULL;
  }
  free(*parent);
  *parent = NULL;

  return 0;
}

int free_item_lst(const node_t *parent, const int index) {
  node_t *ptr;

  ptr = (node_t *)parent;
  for (int i = 0; i < index; i++) {
    if (ptr->next == NULL)
      return 1;
    ptr = ptr->next;
  }
  free_all_lst(&ptr);

  return 0;
}

int delete_child_lst(node_t **parent, const int index) {
  node_t *ptr;
  node_t *tmp_ptr;

  if (index == 0) {
    ptr = *parent;
    *parent = (*parent)->next;
    free(ptr->value);
    free(ptr);
    ptr = NULL;

    return 0;
  }

  ptr = *parent;
  for (int i = 0; i < index - 1; i++) {
    if (ptr->next == NULL)
      return 1;
    ptr = ptr->next;
  }
  tmp_ptr = ptr->next->next;
  free(ptr->value);
  free(ptr->next);
  ptr->next = NULL;
  ptr->next = tmp_ptr;

  return 0;
}
