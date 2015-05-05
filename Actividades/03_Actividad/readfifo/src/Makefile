#    Makefile - Desarrollo de Aplicaciones en Red Servidor
#    all	- Compilar todo
#    clean	- Limpiar archivos compilados
#    rdfifo	- Compilar hello

BIN = ../bin
EXECUTABLES = rdfifo
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
	rm -f $(BIN)/rdfifo *.o

# Compilar los archivos .c
%.o:%.c $(INCLUDES) 
	$(GCC) $(FLAGS) $(CFLAGS) -c $<

# Generar ejecutables individuales
rdfifo: rdfifo.o
	$(GCC) $(FLAGS) $(CFLAGS) -o $(BIN)/rdfifo rdfifo.o $(LIBS)

