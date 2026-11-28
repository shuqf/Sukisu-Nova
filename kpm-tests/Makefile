ifndef TARGET_COMPILE
$(error TARGET_COMPILE not set)
endif

ifndef KP_DIR
KP_DIR = ../..
endif

CC = $(TARGET_COMPILE)gcc
INCLUDE_DIRS := kernel/include kernel/patch/include kernel/linux/include kernel/linux/arch/arm64/include kernel/linux/tools/arch/arm64/include
INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I$(KP_DIR)/$(dir))

all: kpm-symbol-lookup-test.kpm

kpm-symbol-lookup-test.kpm: symbol-lookup.o
	$(CC) -r -o $@ $^

symbol-lookup.o: symbol-lookup.c
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -Tsymbol-test.lds -c -O2 -o $@ $<

clean:
	rm -f *.o *.kpm

.PHONY: all clean
