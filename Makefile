DEBUG = -O0 -ggdb
RELEASE = -O3

EFLAGS = -Wall -Wextra -Werror

MODE = $(DEBUG)

TEST_BUILD = ./tests/build/

test_build:
	mkdir -p $(TEST_BUILD)

test_vmath: tests/vmath_test.c test_build
	cc tests/vmath_test.c $(EFLAGS) $(MODE) -I./include -lm -o $(TEST_BUILD)vmath_test


test_vmesh: tests/vmesh_test.c test_build
	cc tests/vmesh_test.c $(EFLAGS) $(MODE) -I./include -lm -o $(TEST_BUILD)vmesh_test

