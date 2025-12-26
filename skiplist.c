#include "skiplist.h"

static int MAX_HEIGHT = 12;

SkipListNode*
new_skip_list_node(int key, int height) {
  assert(height <= MAX_HEIGHT);

  SkipListNode* node = malloc(sizeof(SkipListNode) + (height - 1) * sizeof(SkipListNode*));
  node->key = key;
  node->height = height;
  return node;
}

SkipList*
new_skip_list() {
  SkipList* skip_list = malloc(sizeof(SkipList));
  skip_list->max_height = 1;
  skip_list->head = new_skip_list_node(0, MAX_HEIGHT);
  return skip_list;
};

SkipListNode*
skip_list_begin(SkipList* skip_list) {
  return skip_list->head->next[0];
}

SkipListNode*
find_node(SkipList* skip_list, int key, SkipListNode** prev_nodes, int prev_nodes_height) {
  SkipListNode* node = skip_list->head;
  int height = skip_list->max_height - 1;
  while (true) {
    SkipListNode* next = node->next[height];
    if (next && key > next->key) {
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
skip_list_lower_bound(SkipList* skip_list, int key) {
  return find_node(skip_list, key, NULL, 0);
}

SkipListNode*
next_skip_list_node(SkipListNode* node) {
  return node->next[0];
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
skip_list_insert(SkipList* skip_list, int key) {
  int height = get_random_height();
  assert(height <= MAX_HEIGHT);

  SkipListNode* node = new_skip_list_node(key, height);

  SkipListNode* x = find_node(skip_list, key, node->next, node->height);
  assert(x ? x->key >= key : true);

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
skip_list_erase(SkipList* skip_list, int key) {
  SkipListNode* prev_nodes[MAX_HEIGHT];
  SkipListNode* x = find_node(skip_list, key, prev_nodes, MAX_HEIGHT);
  if (!x || x->key != key) {
    return 0;
  }

  assert(x->key == key);

  for (int h = 0; h < x->height; h++) {
    prev_nodes[h]->next[h] = x->next[h];
  }

  if (x->height == skip_list->max_height) {
    fix_max_height(skip_list);
  }

  free(x);

  return 1;
}

void
skip_list_release(SkipList* skip_list) {
  SkipListNode* node = skip_list->head->next[0];
  while (node) {
    SkipListNode* next = node->next[0];
    free(node);
    node = next;
  }

  for (int i = 0; i < skip_list->head->height; i++) {
    skip_list->head->next[i] = NULL;
  }

  skip_list->max_height = 1;
}
