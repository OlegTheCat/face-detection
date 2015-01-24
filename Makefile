# include Makefile.opencl
include Makefile.mpi

CC ?= gcc

VALGRIND := valgrind
VALGRIND_OPTS += --tool=memcheck --leak-check=full --trace-children=yes --log-file=valgrind.log.%p

C_OPTS += -Wall -Wextra -Werror -std=c99 $(INCLUDE_DIRS:%=-I%)
ifdef RELEASE
C_OPTS += -O3 -DNDEBUG
else
C_OPTS += -g3 -O0
endif

LIBS += -lm
LD_OPTS += $(LIBS_DIRS:%=-L%) $(LIBS)


BIN_DIR := target/

SRC_DIR := src/
HEADERS := $(wildcard $(SRC_DIR)*.h)
SOURCES := $(wildcard $(SRC_DIR)*.c)
OBJS := $(patsubst $(SRC_DIR)%.c, $(BIN_DIR)%.o, $(SOURCES))
TARGET := libfd.a
BIN_TARGET := $(BIN_DIR)$(TARGET)

$(BIN_DIR) :
	mkdir -p $@

$(BIN_TARGET) : $(OBJS)
	ar rcs $@ $^

$(OBJS) : | $(BIN_DIR)

$(BIN_DIR)%.o : $(SRC_DIR)%.c
	$(CC) -c $< $(C_OPTS) -o $@

lib-target : $(BIN_TARGET)



TEST_DIR := test/
TEST_SOURCES := $(wildcard $(TEST_DIR)*.c)
TEST_OBJS := $(patsubst $(TEST_DIR)%.c, $(BIN_DIR)%.o, $(TEST_SOURCES))
TEST_TARGET := test
BIN_TEST_TARGET := $(BIN_DIR)$(TEST_TARGET)


$(BIN_DIR)%.o : $(TEST_DIR)%.c
	$(CC) -c $< $(C_OPTS) -I$(SRC_DIR) -o $@

$(BIN_TEST_TARGET) : $(BIN_TARGET) $(TEST_OBJS)
	$(CC) $(TEST_OBJS) $(BIN_TARGET) $(LD_OPTS) -o $@

test-target : $(BIN_TEST_TARGET)

test-run : test-target
	$(LAUNCH_OPTS) ./$(BIN_TEST_TARGET)

valgrind-test-run : test-target
	$(LAUNCH_OPTS) $(VALGRIND) $(VALGRIND_OPTS) ./$(BIN_TEST_TARGET)

clean:
	rm -rvf $(BIN_DIR) valgrind.log.*

check-syntax:
	$(CC) $(C_OPTS) -o /dev/null -S ${CHK_SOURCES}

.PHONY: check-syntax clean run target test-target test-run valgrind-test-run
