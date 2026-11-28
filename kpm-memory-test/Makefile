ifndef TARGET_COMPILE
$(error TARGET_COMPILE not set)
endif

ifndef KP_DIR
KP_DIR = ../..
endif

CC = $(TARGET_COMPILE)gcc
INCLUDE_DIRS := kernel/include kernel/patch/include kernel/linux/include kernel/linux/arch/arm64/include kernel/linux/tools/arch/arm64/include
INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I$(KP_DIR)/$(dir))

all: kpm-memory-selftest.kpm

kpm-memory-selftest.kpm: memory-test.o
	$(CC) -r -o $@ $^

memory-test.o: memory-test.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -Tmemory-test.lds -c -O2 -o $@ $<

clean:
	rm -f *.o *.kpm

.PHONY: all clean
