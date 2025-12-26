#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct SkipListNode {
  int key;
  int height;
  struct SkipListNode *next[1];
} SkipListNode;

SkipListNode *new_skip_list_node(int key, int height);

typedef struct SkipList {
  SkipListNode *head;
  int max_height;
} SkipList;

SkipList *new_skip_list();

void print_skip_list(SkipList *skip_list);

SkipListNode *skip_list_begin(SkipList *skip_list);

SkipListNode *skip_list_lower_bound(SkipList *skip_list, int key);

SkipListNode *next_skip_list_node(SkipListNode *node);

void skip_list_insert(SkipList *skip_list, int key);

int skip_list_erase(SkipList *skip_list, int key);

void skip_list_release(SkipList *skip_list);
