#include "skiplist.h"

#include <assert.h>
#include <stdio.h>

void
print_skip_list(SkipList* skip_list) {
  for (int h = skip_list->max_height - 1; h >= 0; h--) {
    SkipListNode* head = skip_list->head->next[h];
    while (head) {
      printf("%d", head->key);
      head = head->next[h];
      if (head) {
        printf(" -> ");
      }
    }
    printf("\n");
  }
}

bool
check_skip_list_order(SkipList* skip_list) {
  SkipListNode* node = skip_list->head->next[0];
  while (node) {
    if (node->next[0] && node->key > node->next[0]->key) {
      return false;
    }
  }
  return true;
}

int
main() {
  int max_value = (int)1e5;

  srand(time(NULL));

  SkipList* skip_list = new_skip_list();

  for (int i = 0; i < 16; i++) {
    int elem = rand() % max_value;
    skip_list_insert(skip_list, elem);
  }

  print_skip_list(skip_list);
  assert(check_skip_list_order(skip_list));

  for (int i = 0; i < 16; i++) {
    int elem = rand() % max_value;
    skip_list_erase(skip_list, elem);
  }

  print_skip_list(skip_list);
  assert(check_skip_list_order(skip_list));

  for (int i = 0; i < 16; i++) {
    int start = rand() % max_value / 2;
    int end = start + rand() % max_value / 2;

    SkipListNode* iterator = skip_list_lower_bound(skip_list, start);
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
