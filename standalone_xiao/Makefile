
OBJS = sample1.o start.o kernel_cfg.o ToppersASP.o

CC = arm-none-eabi-gcc
INCLUDES := -I../src
CFLAGS := -ffunction-sections -fdata-sections -mthumb  -mcpu=cortex-m0plus -g -MD  -Wall -O2 $(INCLUDES) -DARDUINO_SEEED_XIAO_M0
LDFLAGS := -Wl,--gc-sections -L../src/cortex-m0plus/ -nostdlib
LDSCRIPT := ../asp_wo_cfg/target/samd21_gcc/samd21.ld

all: asp.elf

asp.elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(CFLAGS) -T $(LDSCRIPT) -o asp.elf -lkernel -lc  -lgcc

%.o: %.S
	$(CC) -c $(CFLAGS) $<

%.o: %.c
	$(CC) -c $(CFLAGS) $<

%.o: %.cpp
	$(CC) -c $(CFLAGS) -x c $<

#
#  ソースファイルのあるディレクトリに関する定義
#
vpath %.c  ../src/
vpath %.cpp  ../src/

clean:
	-rm *.o asp.elf *.d

#
#  依存関係ファイルをインクルード
#
-include *.d
