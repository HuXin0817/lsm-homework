#include "skiplist.h"
#include <stdio.h>

int main() {
  int max_value = (int)1e5;

  srand(time(NULL));

  SkipList *skip_list = new_skip_list();

  for (int i = 0; i < 16; i++) {
    int elem = rand() % max_value;
    skip_list_insert(skip_list, elem);
  }

  print_skip_list(skip_list);

  for (int i = 0; i < 16; i++) {
    int elem = rand() % max_value;
    skip_list_erase(skip_list, elem);
  }

  print_skip_list(skip_list);

  for (int i = 0; i < 16; i++) {
    int start = rand() % max_value / 2;
    int end = start + rand() % max_value / 2;

    SkipListNode *iterator = skip_list_lower_bound(skip_list, start);
    printf("[ ");
    while (iterator && iterator->key < end) {
      printf("%d ", iterator->key);
      iterator = next_skip_list_node(iterator);
    }
    printf("]\n");
  }

  skip_list_release(skip_list);

  print_skip_list(skip_list);

  return 0;
}
