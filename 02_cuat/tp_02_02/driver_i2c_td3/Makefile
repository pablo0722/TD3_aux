MOD_NAME = i2c_td3
PASSWORD = temppwd
IP = 192.168.6.2

KERNEL_SOURCE := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m += $(MOD_NAME).o
#$(MOD_NAME)-objs := $(MOD_NAME)_cdev.o $(MOD_NAME)_globals.o $(MOD_NAME)_pdev.o






.PHONY: all
all: compile


.PHONY: all
all: rm_mod compile
	
.PHONY: rm_mod
rm_mod:
	sudo dmesg -c
	sudo rmmod ${MOD_NAME}.ko

.PHONY: compile
compile:
	sudo dmesg -c
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} modules
	sudo insmod ${MOD_NAME}.ko
	ls -l /dev/ | grep i2c
	lsmod | grep ${MOD_NAME}
	dmesg

.PHONY: monitor
monitor:
	udevadm trigger
	udevadm monitor --env
	
.PHONY: clean_mod
clean_mod:
	rm *.ko *.o Module.symvers modules.order *.mod.c
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} *.c clean