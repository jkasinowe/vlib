DEBUG = -O0 -ggdb -fsanitize=address
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


test_vvox: tests/vvox_test.c test_build
	cc tests/vvox_test.c $(EFLAGS) $(MODE) -I./include -lm -o $(TEST_BUILD)vvox_test

test_vshape: tests/vshape_test.c test_build
	cc tests/vshape_test.c $(EFLAGS) $(MODE) -I./include -lm -o $(TEST_BUILD)vshape_test

tests: test_vmath test_vmesh test_vvox test_vshape
	$(TEST_BUILD)vmath_test
	$(TEST_BUILD)vmesh_test
	$(TEST_BUILD)vvox_test
	$(TEST_BUILD)vshape_test
