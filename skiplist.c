#include "skiplist.h"

static int MAX_HEIGHT = 12;

SkipListNode*
new_skip_list_node(Str key, Str value, int height) {
  assert(height <= MAX_HEIGHT);

  SkipListNode* node = malloc(sizeof(SkipListNode) + (height - 1) * sizeof(SkipListNode*));
  node->key = str_dup(key);
  node->value = str_dup(value);
  node->height = height;

  for (int h = 0; h < height; h++) {
    node->next[h] = NULL;
  }

  return node;
}

void
skip_list_node_free(SkipListNode* node) {
  str_free(node->key);
  str_free(node->value);
  free(node);
}

SkipList*
new_skip_list() {
  SkipList* skip_list = malloc(sizeof(SkipList));
  skip_list->max_height = 1;
  skip_list->head = new_skip_list_node(new_empty_str(), new_empty_str(), MAX_HEIGHT);
  return skip_list;
};

SkipListNode*
find_node(SkipList* skip_list, Str key, SkipListNode** prev_nodes, int prev_nodes_height) {
  SkipListNode* node = skip_list->head;
  int height = skip_list->max_height - 1;
  while (true) {
    SkipListNode* next = node->next[height];
    if (next && str_compare(key, next->key) == 1) {
      node = next;
      continue;
    }
    if (prev_nodes && height < prev_nodes_height) {
      prev_nodes[height] = node;
    }
    if (height > 0) {
      height--;
    } else {
      assert(height == 0);
      return next;
    }
  }
}

SkipListNode*
skip_list_lower_bound(SkipList* skip_list, Str key) {
  return find_node(skip_list, key, NULL, 0);
}

int
get_random_height() {
  int h = 1;
  while (h < MAX_HEIGHT) {
    if (rand() & 1) {
      h++;
    } else {
      break;
    }
  }
  return h;
}

void
skip_list_insert(SkipList* skip_list, Str key, Str value) {
  int height = get_random_height();
  assert(height <= MAX_HEIGHT);

  SkipListNode* node = new_skip_list_node(key, value, height);

  SkipListNode* x = find_node(skip_list, key, node->next, node->height);
  assert(x ? str_compare(x->key, key) != -1 : true);

  if (x && str_compare(key, x->key) == 0) {
    str_copy(&x->value, value);
    skip_list_node_free(node);
    return;
  }

  if (height > skip_list->max_height) {
    for (int h = skip_list->max_height; h < height; h++) {
      node->next[h] = skip_list->head;
    }

    skip_list->max_height = height;
  }

  for (int h = 0; h < height; h++) {
    SkipListNode* prev_node = node->next[h];
    node->next[h] = prev_node->next[h];
    prev_node->next[h] = node;
  }
}

void
fix_max_height(SkipList* skip_list) {
  for (int h = skip_list->max_height - 1; h >= 0; h--) {
    if (skip_list->head->next[h]) {
      skip_list->max_height = h + 1;
      return;
    }
  }
}

int
skip_list_erase(SkipList* skip_list, Str key) {
  SkipListNode* prev_nodes[MAX_HEIGHT];
  SkipListNode* x = find_node(skip_list, key, prev_nodes, MAX_HEIGHT);
  if (!x || str_compare(x->key, key) != 0) {
    return 0;
  }

  for (int h = 0; h < x->height; h++) {
    prev_nodes[h]->next[h] = x->next[h];
  }

  if (x->height == skip_list->max_height) {
    fix_max_height(skip_list);
  }

  skip_list_node_free(x);

  return 1;
}

void
skip_list_clear(SkipList* skip_list) {
  SkipListNode* node = skip_list->head->next[0];
  while (node) {
    SkipListNode* next = node->next[0];
    skip_list_node_free(node);
    node = next;
  }

  for (int i = 0; i < skip_list->head->height; i++) {
    skip_list->head->next[i] = NULL;
  }

  skip_list->max_height = 1;
}

void
skip_list_free(SkipList* skip_list) {
  skip_list_clear(skip_list);
  skip_list_node_free(skip_list->head);
}
