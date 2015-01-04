CC := gcc
C_OPTS := -Wall -Wextra -g3 -O0
LD_OPTS :=

BIN_DIR := target/
SRC_DIR := src/

HEADERS := $(wildcard $(SRC_DIR)*.h)
SOURCES := $(wildcard $(SRC_DIR)*.c)

OBJS := $(patsubst $(SRC_DIR)%.c, $(BIN_DIR)%.o, $(SOURCES))

TARGET := od

$(BIN_DIR)$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(LD_OPTS)

$(BIN_DIR)%.o : $(SRC_DIR)%.c
	$(CC) -c $< $(C_OPTS) -o $@

target : $(BIN_DIR)$(TARGET)

run : target
	./$(BIN_DIR)$(TARGET)

clean:
	rm -rvf $(BIN_DIR)*

check-syntax:
	$(CC) -Wall -Wextra -o /dev/null -S ${CHK_SOURCES}

.PHONY: check-syntax
