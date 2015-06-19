obj-m:= uartDriver.o

ARCH=x86
CROSS_COMPILE=i586-poky-linux-

all:
	make -C /opt/clanton-full/1.4.2/sysroots/i586-poky-linux/usr/src/kernel M=$(PWD) modules

clean:
	rm -fr *.o
	rm -fr *.ko
	rm -fr *.mod.c
	rm -fr *.order
	rm -fr *.symvers

