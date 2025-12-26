#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "str.h"

typedef struct SkipListNode {
  Str key;
  Str value;
  int height;
  struct SkipListNode* next[1];
} SkipListNode;

#define next_skip_list_node(node) (node->next[0])

typedef struct SkipList {
  SkipListNode* head;
  int max_height;
} SkipList;

SkipList*
new_skip_list();

#define skip_list_begin(skip_list) (skip_list->head->next[0])

SkipListNode*
skip_list_lower_bound(SkipList* skip_list, Str key);

void
skip_list_insert(SkipList* skip_list, Str key, Str value);

int
skip_list_erase(SkipList* skip_list, Str key);

void
skip_list_clear(SkipList* skip_list);

void
skip_list_free(SkipList* skip_list);
