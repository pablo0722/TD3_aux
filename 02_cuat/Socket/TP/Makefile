#CC = /home/pablo/Documentos/Materias/TD_III/Beagle/gcc-linaro-6.4.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
CC = gcc
SRC_SERVER = $(wildcard ./src/server/*.c) $(wildcard ./src/common/*.c)
INC_SERVER = $(wildcard ./inc/server/*.h) $(wildcard ./inc/common/*.h) $(wildcard ./inc/*.h)
OBJ_SERVER = $(subst src/,obj/,$(subst .c,.o,$(SRC_SERVER)))
EXE_SERVER = server

SRC_CLIENT = $(wildcard ./src/client/*.c) $(wildcard ./src/common/*.c)
INC_CLIENT = $(wildcard ./inc/client/*.h) $(wildcard ./inc/common/*.h) $(wildcard ./inc/*.h)
OBJ_CLIENT = $(subst src/,obj/,$(subst .c,.o,$(SRC_CLIENT)))
EXE_CLIENT = client

OPCIONES = -Iinc/common -Iinc/server -Iinc/client -pthread -Wall
DESTINO = ~





.PHONY: all
all: clean client server





.PHONY: client
client: $(OBJ_CLIENT)
	$(CC) $^ -o $(EXE_CLIENT) $(OPCIONES)





.PHONY: server
server: $(OBJ_SERVER)
	$(CC) $^ -o $(EXE_SERVER) $(OPCIONES)





obj/%.o: src/%.c $(INC_SERVER) $(INC_CLIENT)
	$(CC) -c $< -o $@ $(OPCIONES)





.PHONY: crearEstructura
crearEstructura:
	@echo	''
	@echo	'-----> Creando estructura de carpetas'

	mkdir -p src src/client src/server src/common inc inc/client inc/server inc/common obj obj/client obj/server obj/common





.PHONY: clean
clean:
	rm -f $(OBJ_SERVER) $(EXE_SERVER) $(OBJ_CLIENT) $(EXE_CLIENT)





.PHONY: copiar_a_BBB
copiar_a_BBB:
	scp exe debian@192.168.7.2:$(DESTINO)
	ssh debian@192.168.7.2
