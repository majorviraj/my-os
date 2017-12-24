ARMGNU ?= arm-none-eabi
BUILD = build/
SOURCE = source/
TARGET = kernel.img
CFLAGS = -O2 -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -nostartfiles
OBJECTS := $(wildcard $(SOURCE)*.c)
all: $(TARGET)
	
$(TARGET) : $(BUILD)output.elf;
	$(ARMGNU)-objcopy $(BUILD)output.elf -O binary $(TARGET)

$(BUILD)output.elf :$(OBJECTS) $(BUILD);
	$(ARMGNU)-gcc $(CFLAGS) $< -o $@ 

$(BUILD) :;
	mkdir $@

. PHONY: clean

clean:
	-rm -rf $(BUILD)
	-rm -rf $(TARGET)

