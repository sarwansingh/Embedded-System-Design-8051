WORK_DIR :=  F:\DRDO-embedded\RTOS\Multitasking_LCD_LED_Buzzer

#export WORK_DIR
DELETE = tools/rm.exe -f
include make_settings.mak

VPATH = src/appl\
        src/system\
        src/bsp\
        src/os\
        src/hal\
        
APPL_ASM    = src/system/startup.S\
              src/os/os_cpu_a.S\
        
        

OBJECTS_APPL = $(patsubst %.C,$(OBJ_PATH)/%$(OBJECT_SUFFIX),$(foreach dir,$(VPATH),$(wildcard $(dir)/*.c)) )
OBJECTS_APPL  := $(patsubst %.c,$(OBJ_PATH)/%$(OBJECT_SUFFIX),$(notdir $(OBJECTS_APPL)))

#OBJECTS_APPL = $(APPL_SRC:.c=.o)
#ASM_OBJECTS = $(APPL_ASM:.S=.o)
ASM_OBJECTS  = $(patsubst %.S,$(OBJ_PATH)/%$(OBJECT_SUFFIX),$(notdir $(APPL_ASM)))
#ASM_OBJECTS  := $(patsubst %.S,$(OBJ_PATH)/%$(OBJECT_SUFFIX),$(notdir $(OBJECTS_ASM)))


$(BIN_PATH)\$(RELEASE).elf:  appl
	@echo Linking Project...;
	@echo 
	$(CC) $(ALL_CFLAGS) $(ASM_OBJECTS) $(OBJECTS_APPL) -L$(GNU_LIB_PATH) -L$(GCC_LIB_PATH) --output $@  $(LDFLAGS)
	@echo Generating IHex File...
	$(OBJCOPY) -O ihex $(BIN_PATH)/$(RELEASE).elf $(BIN_PATH)/$(RELEASE).hex
	@echo Generating Binary File...;
	@echo 
	$(OBJCOPY) -O binary $(BIN_PATH)/$(RELEASE).elf $(BIN_PATH)/$(RELEASE).bin



appl:$(OBJECTS_APPL) $(ASM_OBJECTS)


$(OBJECTS_APPL) : $(OBJ_PATH)/%$(OBJECT_SUFFIX) : %.c
	@echo
	@echo Compiling $<
	@echo --------------------------------
	$(CC) -c $(ALL_CFLAGS) $(CONLYFLAGS) $(SRC_PATH) $< -o $@


$(ASM_OBJECTS) : $(OBJ_PATH)/%$(OBJECT_SUFFIX) : %.S
	@echo
	@echo Assembling $<
	@echo --------------------------------
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

prog_a:
	@echo Programming Flash...;
	@echo 
	$(MXFLASH) 1 8800000 C000000 $(BIN_PATH)/$(RELEASE)_app.bin

prog_b:
	@echo Programming Flash...;
	@echo 
	$(MXFLASH) 1 8800000 C400000 $(BIN_PATH)/$(RELEASE)_set.bin



#rule to clean up the generated files
.PHONY: clean
clean:
	$(DELETE) build/lst/*.lst
	$(DELETE) build/obj/*.o
	$(DELETE) build/bin/*.*

#first clean, the make
.PHONY: cleanmake
cleanmake:
	$(MAKE) clean
	$(MAKE)


.PHONY: test
test:
	@echo all obj :$(ASM_OBJECTS)
