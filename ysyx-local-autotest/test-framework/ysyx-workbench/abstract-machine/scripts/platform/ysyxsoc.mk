AM_SRCS := riscv/ysyxsoc/start.S \
           riscv/ysyxsoc/trm.c \
           riscv/ysyxsoc/ioe.c \
           riscv/ysyxsoc/timer.c \
           riscv/ysyxsoc/input.c \
           riscv/ysyxsoc/uart.c \
           riscv/ysyxsoc/gpu.c \
           riscv/ysyxsoc/cte.c \
           riscv/ysyxsoc/trap.S \
           platform/dummy/vme.c \
           platform/dummy/mpe.c

CFLAGS    += -fdata-sections -ffunction-sections
LDFLAGS   += -T $(AM_HOME)/scripts/ysyxsoc_linker.ld \
						 --defsym=_pmem_start=0x30000000 --defsym=_entry_offset=0x0 --print-map > map.txt
LDFLAGS   += --gc-sections -e _start
CFLAGS += -DMAINARGS=\"$(mainargs)\"
.PHONY: $(AM_HOME)/am/src/riscv/ysyxsoc/trm.c

image: $(IMAGE).elf
	@$(OBJDUMP) -d $(IMAGE).elf > $(IMAGE).txt
	@echo + OBJCOPY "->" $(IMAGE_REL).bin
	@$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(IMAGE).elf $(IMAGE).bin

run: image
ifneq ($(strip $(FILTERED_IMAGE_LOG)),)
	$(MAKE) -C $(NPC_HOME) ISA=$(ISA) run IMG=$(IMAGE).bin > $(LOG_DIR)/$(FILTERED_IMAGE_LOG) 2>&1
else
	@echo $(NPC_HOME) $(ISA) $(IMAGE).bin $(LOG_DIR)
	$(MAKE) -C $(NPC_HOME) ISA=$(ISA) run IMG=$(IMAGE).bin
endif

#gdb: image
	#$(MAKE) -C $(NPC_HOME) ISA=$(ISA) gdb  IMG=$(IMAGE).bin
