DEBUG = -O0 -g -gdwarf-4 -fsanitize=address -fno-omit-frame-pointer
RELEASE = -O3

EFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable

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

test_vstruct: tests/vstruct_test.c test_build
	cc tests/vstruct_test.c $(EFLAGS) $(MODE) -I./include -lm -o $(TEST_BUILD)vstruct_test

test_vmem: tests/vmem_test.c test_build
	cc tests/vmem_test.c $(EFLAGS) $(MODE) -I./include -lm -o $(TEST_BUILD)vmem_test

test_vsys: tests/vsys_test.c test_build
	cc tests/vsys_test.c $(EFLAGS) $(MODE) -I./include -lm -o $(TEST_BUILD)vsys_test -lSDL3 -lvolk

test_vfs: tests/vfs_test.c test_build
	cc tests/vfs_test.c $(EFLAGS) $(MODE) -I./include -lm -o $(TEST_BUILD)vfs_test -lSDL3 -lvolk


tests: test_vmem test_vmath test_vmesh test_vvox test_vshape test_vstruct
	$(TEST_BUILD)vmem_test
	$(TEST_BUILD)vmath_test
	$(TEST_BUILD)vmesh_test
	$(TEST_BUILD)vvox_test
	$(TEST_BUILD)vshape_test
	$(TEST_BUILD)vstruct_test
