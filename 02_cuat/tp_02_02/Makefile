#CC = /home/pablo/Documentos/TD_III/Beagle/gcc-linaro-6.4.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
CC = gcc
SRC_SERVER = $(wildcard ./src/server/*.c) $(wildcard ./src/common/*.c)
INC_SERVER = $(wildcard ./inc/server/*.h) $(wildcard ./inc/common/*.h) $(wildcard ./inc/*.h)
OBJ_SERVER = $(subst src/,obj/,$(subst .c,.o,$(SRC_SERVER)))
EXE_SERVER = server

SRC_CLIENT = $(wildcard ./src/client/*.c) $(wildcard ./src/common/*.c)
INC_CLIENT = $(wildcard ./inc/client/*.h) $(wildcard ./inc/common/*.h) $(wildcard ./inc/*.h)
OBJ_CLIENT = $(subst src/,obj/,$(subst .c,.o,$(SRC_CLIENT)))
EXE_CLIENT = client

SRC_TEST = $(wildcard ./src/*.c)
INC_TEST = $(wildcard ./inc/*.h)
OBJ_TEST = $(subst src/,obj/,$(subst .c,.o,$(SRC_TEST)))
EXE_TEST = test

OPCIONES = -Iinc/common -Iinc/server -Iinc/client -Iinc -pthread -Wall
DESTINO = ~





.PHONY: all
all: clean test client server





.PHONY: client
client: $(OBJ_CLIENT)
	@echo	''
	@echo 	'Creando cliente...'
	$(CC) $^ -o $(EXE_CLIENT) $(OPCIONES)





.PHONY: server
server: $(OBJ_SERVER)
	@echo	''
	@echo 	'Creando servidor...'
	$(CC) $^ -o $(EXE_SERVER) $(OPCIONES)





.PHONY: test
test: $(OBJ_TEST)
	@echo	''
	@echo 	'Creando prueba...'
	$(CC) $^ -o $(EXE_TEST) $(OPCIONES)





obj/%.o: src/%.c $(INC_SERVER) $(INC_CLIENT) $(INC_TEST)
	$(CC) -c $< -o $@ $(OPCIONES)





.PHONY: crearEstructura
crearEstructura:
	@echo	''
	@echo	'-----> Creando estructura de carpetas'

	mkdir -p src src/client src/server src/common inc inc/client inc/server inc/common obj obj/client obj/server obj/common





.PHONY: clean
clean:
	@echo	''
	@echo 	'Limpiando objetos y ejecutables...'
	rm -f $(OBJ_SERVER) $(EXE_SERVER) $(OBJ_CLIENT) $(EXE_CLIENT) $(OBJ_TEST) $(EXE_TEST)





.PHONY: download_headers
download_headers:
	@echo	''
	@echo 	'sudo apt-get install linux-headers-$(uname -r)'





.PHONY: replace_device_tree
replace_device_tree:
	@echo dtc -I dts -O dtb -o am335x-boneblack.dtb am335x.dts
	@echo sudo cp am335x-boneblack.dtb /boot/dtbs/4.14.71-ti-r80/





.PHONY: check_device_tree
check_device_tree:
	ls /proc/device-tree/ocp | grep i2c





.PHONY: check_mem
check_mem:
	@echo "checkear /var/ /tmp/ /sys/ /proc/ /opt/"
	@echo du -sh * | sort -h
	










PASSWORD = temppwd
IP = 192.168.6.2

.PHONY: copy
copy:
	@echo	''
	@echo 	'Copiando a la Beagle...'
	sshpass -p $(PASSWORD) ssh debian@$(IP) "rm -rf ~/*"
	sshpass -p $(PASSWORD) scp -r ./driver_i2c_td3 ./info/am335x.dts ./inc ./assets ./src ./obj ./$(EXE_SERVER) ./$(EXE_CLIENT) ./$(EXE_TEST) ./Makefile debian@$(IP):~
	sshpass -p $(PASSWORD) ssh debian@$(IP)