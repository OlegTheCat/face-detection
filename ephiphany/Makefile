CC := gcc
ECC := e-gcc

ESDK := $(EPIPHANY_HOME)
ELIBS:= $(ESDK)/tools/host/lib
EINCS := $(ESDK)/tools/host/include
ELDF := $(ESDK)/bsps/current/internal.ldf
EHDF := $(EPIPHANY_HDF)

BIN_DIR := bin/
SRC_DIR := src/

HEADERS := $(shell find $(SRC_DIR) -iname "*.h")
DEVICE_SOURCES := $(shell find $(SRC_DIR) -iname "e_*.c")
HOST_SOURCES := $(shell find $(SRC_DIR) ! -iname "e_*.c" -iname "*.c")

DEVICE_OBJS := $(patsubst %.c,%.o,$(notdir $(DEVICE_SOURCES)))
HOST_OBJS := $(patsubst %.c,%.o,$(notdir $(HOST_SOURCES)))

TARGET := e-face-detection

default : run

$(BIN_DIR) :
	mkdir -p $(BIN_DIR)

host : $(HOST_SOURCES) $(BIN_DIR)
	cd $(BIN_DIR) && \
	$(CC) -c $(addprefix ../, $(HOST_SOURCES)) -I $(EINCS)

device : $(DEVICE_SOURCES) $(BIN_DIR)
	cd $(BIN_DIR) && \
	$(ECC) -O3 -c $(addprefix ../, $(DEVICE_SOURCES)) && \
	$(ECC) -T $(ELDF) -o $(TARGET).elf $(DEVICE_OBJS) -le-lib -lm -ffast-math && \
	e-objcopy --srec-forceS3 --output-target srec $(TARGET).elf $(TARGET).srec

target : host device
	cd $(BIN_DIR) && \
	$(CC) -o $(TARGET) $(HOST_OBJS) -L $(ELIBS) -le-hal

run : target
	cd $(BIN_DIR) && \
	sudo -E LD_LIBRARY_PATH=$(ELIBS) EPIPHANY_HDF=$(EHDF) ./$(TARGET)

clean:
	rm -rfv $(BIN_DIR)
