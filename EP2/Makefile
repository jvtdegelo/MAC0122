# Ajuste os diretorios abaixo conforme o seu sistema 
LIBDIR = /C:/Users/DELL/Documents/POLI/MAC0122/EP2 
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-unused-result
###########################################################################

all: testa-EP2

testa-EP2: testa-EP2.o polinomios.o 
	gcc -o testa-EP2 testa-EP2.o polinomios.o

testa-EP2.o: testa-EP2.c polinomios.h
	gcc $(CFLAGS) testa-EP2.c -c 

ep3: ep3.o polinomios.o
	gcc -o ep3 ep3.o polinomios.o

ep3.o: ep3.c polinomios.h
	gcc $(CFLAGS) ep3.c -c 

polinomios.o: polinomios.c polinomios.h
	gcc $(CFLAGS) polinomios.c -c

clean:
	rm -rf *.o *~ client
