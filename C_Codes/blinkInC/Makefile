ARMGNU ?= arm-none-eabi
BUILD = build/
SOURCE = source/
LIBRARIES = ../../libraries/
LIST = kernel.list
SYMBOLS = kernel.nm
TARGET = kernel.img
UPLOADSCRIPT = ./run.sh
CFLAGS = -O2 -mfpu=vfp -mfloat-abi=softfp -march=armv6zk -mtune=arm1176jzf-s -nostartfiles -g -Wl,-T,linkerScript.ld
OBJECTS := $(wildcard $(SOURCE)*.c) $(wildcard $(SOURCE)*.s) $(wildcard $(LIBRARIES)*.s) $(wildcard $(LIBRARIES)*.a)

all: $(TARGET) $(LIST)

$(TARGET) : $(BUILD)output.elf
	$(ARMGNU)-objcopy $(BUILD)output.elf -O binary $(TARGET)

$(LIST) : $(BUILD)output.elf
	$(ARMGNU)-objdump -D $(BUILD)output.elf > $(LIST)
	$(ARMGNU)-nm $(BUILD)output.elf > $(SYMBOLS)

$(BUILD)output.elf: $(OBJECTS) $(BUILD)
	$(ARMGNU)-gcc $(CFLAGS) -I$(LIBRARIES) $(OBJECTS) -o $(BUILD)output.elf

$(BUILD) :
	mkdir $@

.PHONY: debug clean program all

program:
	$(UPLOADSCRIPT)

debug:
	gdbgui --gdb $(ARMGNU)-gdb 'build/output.elf'

clean:
	-rm -rf $(BUILD)
	-rm -f $(TARGET)
	-rm -f $(SYMBOLS)
	-rm -f $(LIST)
