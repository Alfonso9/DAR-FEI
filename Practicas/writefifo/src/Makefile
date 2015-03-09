#    Makefile - Desarrollo de Aplicaciones en Red Servidor
#    all	- Compilar todo
#    clean	- Limpiar archivos compilados
#    wrfifo	- Compilar hello

BIN = ../bin
EXECUTABLES = wrfifo
# Agregar cabeceras .h
INCLUDES = 
#Usar gcc
GCC = gcc 
#Bibliotecas
LIBS = -lm -lpthread
#FLAGS =  -std=c99
CFLAGS = -I.

#'make all' creará todos los ejecutables
all: 	$(EXECUTABLES)

#'make clean' eleminar archivos objetos
clean:
	rm -f $(BIN)/wrfifo *.o

# Compilar los archivos .c
%.o:%.c $(INCLUDES) 
	$(GCC) $(FLAGS) $(CFLAGS) -c $<

# Generar ejecutables individuales
wrfifo: wrfifo.o
	$(GCC) $(FLAGS) $(CFLAGS) -o $(BIN)/wrfifo wrfifo.o $(LIBS)

