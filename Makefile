CC = gcc

CFLAGS = -Wall -Wextra -g -std=c99

TEST_SRCS = $(wildcard *_test.c)
TEST_BINS = $(TEST_SRCS:_test.c=)

ALL_SRCS = $(wildcard *.c)
ALL_OBJS = $(ALL_SRCS:.c=.o)

COMMON_SRCS = str.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

all: $(TEST_BINS)

define BUILD_TEST
ifneq ($$(wildcard $(1).c),)
$(1): $(1)_test.o $(1).o $$(filter-out $(1).o,$(COMMON_OBJS))
	$$(CC) $$(CFLAGS) -o $$@ $$^
else
$(1): $(1)_test.o $(COMMON_OBJS)
	$$(CC) $$(CFLAGS) -o $$@ $$^
endif
endef

$(foreach test,$(TEST_BINS),$(eval $(call BUILD_TEST,$(test))))

%_test.o: %_test.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TEST_BINS) $(ALL_OBJS)

rebuild: clean all

.PHONY: all clean rebuild
