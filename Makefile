CC := gcc
C_OPTS := -Wall -Wextra -g3 -O0
LD_OPTS := -lm


BIN_DIR := target/

SRC_DIR := src/
HEADERS := $(wildcard $(SRC_DIR)*.h)
SOURCES := $(wildcard $(SRC_DIR)*.c)
OBJS := $(patsubst $(SRC_DIR)%.c, $(BIN_DIR)%.o, $(SOURCES))
TARGET := libod.a
BIN_TARGET := $(BIN_DIR)$(TARGET)


$(BIN_TARGET) : $(OBJS)
	ar rcs $@ $^

$(BIN_DIR)%.o : $(SRC_DIR)%.c
	$(CC) -c $< $(C_OPTS) -o $@

target : $(BIN_TARGET)



TEST_DIR := test/
TEST_SOURCES := $(wildcard $(TEST_DIR)*.c)
TEST_OBJS := $(patsubst $(TEST_DIR)%.c, $(BIN_DIR)%.o, $(TEST_SOURCES))
TEST_TARGET := test
BIN_TEST_TARGET := $(BIN_DIR)$(TEST_TARGET)


$(BIN_DIR)%.o : $(TEST_DIR)%.c
	$(CC) -c $< $(C_OPTS) -I$(SRC_DIR) -o $@

$(BIN_TEST_TARGET) : $(TEST_OBJS) $(BIN_TARGET)
	$(CC) $^ $(LD_OPTS) -o $@

test-target : $(BIN_TEST_TARGET)

test-run : test-target
	./$(BIN_TEST_TARGET)

valgrind-test-run : test-target
	valgrind --tool=memcheck --leak-check=full ./$(BIN_TEST_TARGET)

clean:
	rm -rvf $(BIN_DIR)*

check-syntax:
	$(CC) $(C_OPTS) -o /dev/null -S ${CHK_SOURCES}

.PHONY: check-syntax clean run target test-target test-run
