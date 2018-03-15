ARMGNU ?= arm-none-eabi
BUILD = build/
SOURCE = source/
LIBRARIES = ../../libraries/
C_LIBRARIES = ../../libraries/includes/
LIST = kernel.list
SYMBOLS = kernel.nm
TARGET = kernel.img
UPLOADSCRIPT = ./run.sh
CFLAGS = -O1 -mfpu=vfp -mfloat-abi=softfp -march=armv6zk -mtune=arm1176jzf-s -nostartfiles -std=gnu11 -ggdb -Wl,-T,linkerScript.ld,--no-wchar-size-warning
OBJECTS := $(wildcard $(SOURCE)*.c) $(wildcard $(SOURCE)*.s) $(wildcard $(LIBRARIES)*.s) $(wildcard $(LIBRARIES)*.a) $(wildcard $(C_LIBRARIES)*.c) 

all: $(TARGET) $(LIST)

$(TARGET) : $(BUILD)output.elf
	$(ARMGNU)-objcopy $(BUILD)output.elf -O binary $(TARGET)

$(LIST) : $(BUILD)output.elf
	$(ARMGNU)-objdump -D $(BUILD)output.elf > $(LIST)
	$(ARMGNU)-nm $(BUILD)output.elf > $(SYMBOLS)

$(BUILD)output.elf: $(OBJECTS) $(BUILD)
	$(ARMGNU)-gcc $(CFLAGS) -I$(C_LIBRARIES) $(OBJECTS) -o $(BUILD)output.elf

$(BUILD) :
	mkdir $@

.PHONY: debug clean program all

program:
	$(UPLOADSCRIPT)

jtag:
	openocd -f openocd.cfg

clean:
	-rm -rf $(BUILD)
	-rm -f $(TARGET)
	-rm -f $(SYMBOLS)
	-rm -f $(LIST)
