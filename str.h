#include <stdint.h>

typedef struct Str {
  uint16_t size;
  char* data;
} Str;

Str
new_str(uint16_t size);

Str
new_empty_str();

Str
str_dup(const Str from);

void
str_copy(Str* str, const Str from);

int
str_compare(const Str a, const Str b);

void
str_free(Str str);
