#include "skiplist.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void
print_skip_list(SkipList* skip_list) {
  for (int h = skip_list->max_height - 1; h >= 0; h--) {
    SkipListNode* head = skip_list->head->next[h];
    while (head) {
      printf("%s", head->key.data);
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
    if (node->next[0] && str_compare(node->key, node->next[0]->key) == 1) {
      return false;
    }

    node = node->next[0];
  }
  return true;
}

Str
get_random_str() {
  int size = rand() % 5 + 5;

  const char letters[] = "abcdefghijklmnopqrstuvwxyz";
  const int letters_len = sizeof(letters) - 1;

  char* str = malloc((size + 1) * sizeof(char));

  for (int i = 0; i < size; i++) {
    int random_idx = rand() % letters_len;
    str[i] = letters[random_idx];
  }
  str[size] = '\0';

  return (Str){
      .data = str,
      .size = size,
  };
}

int
main() {
  srand(time(NULL));

  SkipList* skip_list = new_skip_list();

  for (int i = 0; i < 16; i++) {
    Str key = get_random_str();
    Str value = get_random_str();
    skip_list_insert(skip_list, get_random_str(), get_random_str());

    str_free(key);
    str_free(value);
  }

  print_skip_list(skip_list);
  assert(check_skip_list_order(skip_list));

  for (int i = 0; i < 16; i++) {
    Str key = get_random_str();
    skip_list_erase(skip_list, key);

    str_free(key);
  }

  print_skip_list(skip_list);
  assert(check_skip_list_order(skip_list));

  for (int i = 0; i < 16; i++) {
    Str key = get_random_str();

    SkipListNode* iterator = skip_list_lower_bound(skip_list, key);
    printf("[ ");
    while (iterator) {
      printf("%s ", iterator->key.data);
      iterator = next_skip_list_node(iterator);
    }
    printf("]\n");

    str_free(key);
  }

  skip_list_free(skip_list);

  return 0;
}
