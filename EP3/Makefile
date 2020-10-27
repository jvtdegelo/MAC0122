# Ajuste os diretorios abaixo conforme o seu sistema 
LIBDIR = /C:/Users/DELL/Documents/POLI/MAC0122/EP3 
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-unused-result
###########################################################################

all: ep3

ep3: ep3.o polinomios.o stackS.o
	gcc ep3.o polinomios.o stackS.o -o ep3

ep3.o: ep3.c polinomios.h stackS.h item.h
	gcc $(CFLAGS) -c ep3.c 

stackS.o: stackS.c stackS.h item.h
	gcc $(CFLAGS) -c stackS.c

polinomios.o: polinomios.c polinomios.h
	gcc $(CFLAGS) -c polinomios.c
	
clean:
	rm -rf *.o *~ client
