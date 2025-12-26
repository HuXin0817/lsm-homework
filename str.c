#include "str.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

Str
new_str(uint16_t size) {
  Str str;
  str.size = size;

  if (size > 0) {
    str.data = malloc(size);
  } else {
    str.data = NULL;
  }

  return str;
}

Str
new_empty_str() {
  return new_str(0);
}

Str
str_dup(const Str from) {
  Str str = new_empty_str();
  str_copy(&str, from);
  return str;
}

void
str_copy(Str* str, const Str from) {
  if (str->size > 0) {
    assert(str->data);
    free(str->data);
  }

  if (from.size > 0) {
    assert(from.data);

    str->size = from.size;
    str->data = malloc(from.size);
    memcpy(str->data, from.data, from.size);
  } else {
    assert(from.data == NULL);

    str->size = 0;
    str->data = NULL;
  }
}

int
str_compare(const Str a, const Str b) {
  uint16_t min_size = min(a.size, b.size);

  for (uint16_t i = 0; i < min_size; i++) {
    if (a.data[i] < b.data[i]) {
      return -1;
    }
    if (a.data[i] > b.data[i]) {
      return 1;
    }
  }

  if (a.size < b.size) {
    return -1;
  }
  if (a.size > b.size) {
    return 1;
  }
  return 0;
}

void
str_free(Str str) {
  if (str.size) {
    assert(str.data);
    free(str.data);
  }
}