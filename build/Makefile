
OBJS = sample1.o start.o kernel_cfg.o ToppersASP.o

CC = arm-none-eabi-gcc
INCLUDES := -I../src
CFLAGS := -ffunction-sections -fdata-sections -mthumb  -mcpu=cortex-m4 -g -MD  -Wall -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 $(INCLUDES) 
LDFLAGS := -Wl,--gc-sections -L../src/cortex-m4/ -nostdlib
LDSCRIPT := ../asp_wo_cfg/target/samd51_gcc/samd51.ld

all: asp.elf

asp.elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -T $(LDSCRIPT) -o asp.elf -lkernel -lc  -lgcc

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
