# Makefile para TD3 UTN FRBA
# Version: 3.0
# Año: 2018

# NOTA: Los paths relativos deben especificarse SIN el "./"
#		Los archivos assembler deben tener extension .asm 










# ******************************************************************** #
# ************* Definiciones modificables por el usuario ************* #
# ******************************************************************** #

# Path donde el BOCHS irá a buscar el binario (así, una vez generado, copia el binario a ese path y se puede ejecutar el bochs directamente sin modificar nada)
PATH_BIOS = /home/pablo/UTN/TD3/bochs

# Carpeta donde se encuentran los archivos fuente (.asm)
CARPETA_SRC = src

# Carpeta donde se desea generar el archivo binario (.bin)
CARPETA_BIN = bin

# Carpeta donde se desea generar el archivo de Formato Ejecutable y Linkeable (.elf)
CARPETA_ELF = bin

# Carpeta donde se desea generar el archivo de lista del binario (.lst)
CARPETA_LST = bin

# Carpeta donde se desean generar los archivos objeto (.o)
CARPETA_OBJ = bin

# Carpeta donde se desean generar los archivos de texto para control (.txt)
CARPETA_TXT = bin

# Nombre del archivo binario generado
OUTPUT_NAME = bios

# Flags del compilador
# "-felf32": Formato de salida elf 32 bits
CFLAGS = -Iinc/ -Wall
#CFLAGS = -felf32 -Wall

# Flags del linker
# "-z max-page-size=0x01000": Tamaño maximo de pagina 
# "-m elf_i386": Emula un procesador i386
LFLAGS = -z max-page-size=0x01000 -m elf_i386

# Flags del objdump
# "-C": Decodifica los nombres de símbolo
# "-p": Display object format specific file header contents
# "-r": Display the relocation entries in the file
# "-s": Muestra el contenido completo de todas las secciones pedidas
# "-S": Muestra código fuente junto con código desensamblado 
# "-x": Muestra contenido de todos los headers
# "--prefix-addresses": Muestra las direcciones completas a lo largo del desensamblado
ODFLAGS	= -C -p -r -s -S -x --prefix-addresses










# ******************************************************************** #
# ************* NO modificar las siguientes definiciones ************* #
# ******************************************************************** #

# Compilador
COMPILER = nasm

# Linker
LINKER = ld

# Path y nombre del binario (.bin)
OUTPUT_BIN = $(CARPETA_BIN)/$(OUTPUT_NAME).bin

# Path y nombre del archivo de salida en formato linkeable y ejecutable (.elf)
OUTPUT_ELF = $(CARPETA_ELF)/$(OUTPUT_NAME).elf

# Path y nombre de los archivos fuente (.asm)
SRCS = $(wildcard $(CARPETA_SRC)/*.asm)

# Path y nombre de los archivos objeto (.o)
OBJS = $(subst $(CARPETA_SRC)/,$(CARPETA_OBJ)/,$(SRCS:.asm=.o))

# Path y nombre del binadio generado
BIOS = $(PATH_BIOS)/$(OUTPUT_NAME).bin

# Define una funcion para crear la estructura de carpetas
define CREAR_ESTRUCTURA
	@echo	''
	@echo	'-----> Creando estructura de carpetas'

	mkdir -p ./bin
	mkdir -p ./doc
	mkdir -p ./inc
	mkdir -p ./lib
	mkdir -p ./src
	mkdir -p ./sup
	mkdir -p ./log
endef

# Define una funcion para crear la estructura de carpetas
define COPY_BIOS
	@echo	''
	@echo	'-----> Copiando bios al path del bochs'

	cp $(OUTPUT_BIN) $(BIOS)
endef









# ******************************************************************** #
# ****************************** Reglas ****************************** #
# ******************************************************************** #

all: clean compilar
	$(COPY_BIOS)
	@echo	''
	@echo	'Ejecutando bochs'
	bochs -q
	rm *.ini

$(OUTPUT_BIN): $(OBJS)
	@echo	''
	@echo	'-----> Linkeando' $^ 'a binario'

	#$(LINKER) $(LFLAGS) --oformat=binary $^ -o $@

	@echo	''
	@echo	'tamaño en bytes:'
	stat -c%s "$(OUTPUT_BIN)"

$(OUTPUT_ELF): $(OBJS)
	@echo	''
	@echo	'-----> Linkeando' $^ 'a ELF'

	$(LINKER) $(LFLAGS) $^ -o $@

./$(CARPETA_OBJ)/%.o: ./$(CARPETA_SRC)/%.asm
	@echo	''
	@echo	'-----> Compilando fuente' $<

	$(COMPILER) $(CFLAGS) $< -o $(OUTPUT_BIN) -l $(subst $(CARPETA_OBJ)/,$(CARPETA_LST)/,$(@:.o=.lst))


.PHONY: compilar
compilar: clean $(OUTPUT_BIN)
	@echo	''
	@echo	'-----> Generando listados de hexadecimales y ELFs'

	hexdump -C $(OUTPUT_BIN) > ./$(CARPETA_TXT)/$(OUTPUT_NAME)_hexdump.txt
	#objdump $(ODFLAGS) $(OUTPUT_ELF) > ./$(CARPETA_TXT)/$(OUTPUT_NAME)_objdump.txt
	#readelf -a $(OUTPUT_ELF) > ./$(CARPETA_TXT)/$(OUTPUT_NAME)_readelf.txt

	@echo	''
	@echo	''
	@echo	'LISTO! n_n'
	@echo	''

.PHONY: copyBios
copyBios:
	$(COPY_BIOS)


.PHONY: crearEstructura
crearEstructura:
	$(CREAR_ESTRUCTURA)

clean:
	@echo	''
	@echo	'-----> Limpiando'

	rm -f *.o 
	rm -f ./$(CARPETA_BIN)/*.bin
	rm -f ./$(CARPETA_ELF)/*.elf
	rm -f ./$(CARPETA_OBJ)/*.o
	rm -f ./$(CARPETA_TXT)/*.txt
	rm -f ./$(CARPETA_LST)/*.lst
	rm -f ./$(CARPETA_BIN)/*.log
	rm -f ./$(CARPETA_BIN)/*.ini