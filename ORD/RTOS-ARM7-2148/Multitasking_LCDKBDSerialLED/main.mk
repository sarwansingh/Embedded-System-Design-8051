PRG		= main
OBJ		= Startup.o app.o bsp.o
LDSCRIPT	= main.ld
GNUARMPATH	= C:\SCARM\GNUARM
INCPATH1	= 
INCPATH2	= C:\SCARM\GNUARM\arm-elf\include


# OPTIMIZE       = -g

DEBUG		= -g2

# USE_THUMB_MODE = YES



DEFS           =

LIBS           =



CFLAGS2	= -mcpu=arm7tdmi

#CFLAGS2	= -mcpu=cortex-m3 -mfpu=vfp -mthumb



ASFLAGS2 = 

#ASFLAGS2 = -mcpu=cortex-m3 -mfpu=vfp



# You should not have to change anything below here.

CC             = arm-elf-gcc

OBJCOPY        = arm-elf-objcopy

OBJDUMP        = arm-elf-objdump

ARCH           = arm-elf-ar



CFLAGS=-Wall -I. -I$(INCPATH1) -I$(INCPATH2) $(DEBUG) -nostartfiles -nostdlib -T$(LDSCRIPT) \

		-Wcast-align -Wa,-alhms=$<.lst $(OPTIMIZE) $(DEFS)



LDFLAGS=



ifeq ($(USE_THUMB_MODE),YES)

	CFLAGS += -mthumb -mthumb-interwork

endif



ASFLAGS=-Wa -Wa,--gdwarf-2 $(DEBUG)



hex:  $(PRG).hex



%.hex: %.elf

	$(OBJCOPY) -O ihex $< $@



$(PRG).elf: $(OBJ) $(LDSCRIPT)

	$(CC) $(CFLAGS) $(CFLAGS2) $(LDFLAGS) $(OBJ) $(LIBS) $(GNUARMPATH)\arm-elf\lib\libc.a $(GNUARMPATH)\arm-elf\lib\libm.a $(GNUARMPATH)\lib\gcc\arm-elf\4.4.1\libgcc.a -o $@

#	$(CC) $(CFLAGS) $(CFLAGS2) $(LDFLAGS) -o $@



lst:  $(PRG).lst



%.lst: %.elf

	$(OBJDUMP) -h -S $< > $@



%.o : %.s

	$(CC) -c $(ASFLAGS) $(ASFLAGS2) $< -o $@



%.o : %.c

	$(CC) -c $(CFLAGS) $(CFLAGS2) $< -o $@



bin: $(PRG).bin



%.bin: %.elf

	$(OBJCOPY) -O binary $< $@



clean:

	rm $(OBJ)

	rm $(PRG).hex

	rm $(PRG).elf

