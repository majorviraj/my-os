ARMGNU ?= arm-none-eabi
BUILD = build/
SOURCE = source/
LIST = kernel.list
SYMBOLS = kernel.nm
TARGET = kernel.img
CFLAGS = -O2 -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -nostartfiles -g -Wl,-T,arm-gccDefault_ld.ld
OBJECTS := $(wildcard $(SOURCE)*.c)
all: $(TARGET) $(LIST)
	
$(TARGET) : $(BUILD)output.elf;
	$(ARMGNU)-objcopy $(BUILD)output.elf -O binary $(TARGET)

$(LIST) : $(BUILD)output.elf
	$(ARMGNU)-objdump -D $(BUILD)output.elf > $(LIST)
	$(ARMGNU)-nm $(BUILD)output.elf > $(SYMBOLS)

$(BUILD)output.elf :$(OBJECTS) $(BUILD);
	$(ARMGNU)-gcc $(CFLAGS) $< -o $@ 

$(BUILD) :;
	mkdir $@

.PHONY: debug 
debug:
	gdbgui --gdb $(ARMGNU)-gdb 'build/output.elf'

.PHONY: clean

clean:
	-rm -rf $(BUILD)
	-rm -f $(TARGET)
	-rm -f $(SYMBOLS)
	-rm -f $(LIST)
