MOD_NAME = cind
IP = 192.168.6.2
PASSWORD = temppwd





ifneq (${KERNELRELEASE},)
	obj-m := ${MOD_NAME}.o
else
	KERNEL_SOURCE := /usr/src/linux-headers-$(shell uname -r)
	PWD := $(shell pwd)

compile_mod:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} ${MOD_NAME}.c modules

install_mod:
	sudo insmod ${MOD_NAME}.ko

monitor_mod:
	udevadm trigger
	udevadm monitor --env

list_mods:
	lsmod | grep ${MOD_NAME}

debug_mod:
	dmesg

clean_dmseg:
	sudo dmesg -c
	
rm_mod:
	rm *.ko *.o Module.symvers modules.order *.mod.c
	sudo rmmod ${MOD_NAME}.ko


copy:
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf ~/cind"
	sshpass -p $(PASSWORD) scp -r ../cind/ debian@$(IP):~/cind
	sshpass -p $(PASSWORD) ssh debian@$(IP)
endif