# Makefile to build light_ws2812 library examples
# This is not a very good example of a makefile - the dependencies do not work, therefore everything is rebuilt every time.

F_CPU = 20000000
DEVICE = attiny2313

CC   = avr-gcc
LIB  = ws2812
APPS = light
DEP  = ws2812_config.h WS2812/ws2812.h

CFLAGS = -g2 -I. -IWS2812 -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) 
CFLAGS+= -Os -ffunction-sections -fdata-sections -fpack-struct -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions  
CFLAGS+= -Wall -Wno-pointer-to-int-cast
#CFLAGS+= -Wa,-ahls=$<.lst

LDFLAGS = -Wl,--relax,--section-start=.text=0,-Map=main.map

all:	$(APPS) 

$(LIB): $(DEP)
	@echo Building Library 
	@$(CC) $(CFLAGS) -o objects/$@.o -c WS2812/$@.c 

$(APPS): $(LIB) 
	@echo Building $@
	@$(CC) $(CFLAGS) -o objects/$@.o applications/$@.c WS2812/$^.c
	@avr-size objects/$@.o
	@avr-objcopy -j .text  -j .data -O ihex objects/$@.o $@.hex
	@avr-objdump -d -S objects/$@.o >objects/$@.lss

.PHONY:	clean

clean:
	rm -f *.hex objects/*.o objects/*.lss
