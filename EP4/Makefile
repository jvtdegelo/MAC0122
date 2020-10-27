LIBDIR = /C:/Users/DELL/Documents/POLI/MAC0122/EP4 
CFLAGS = -g -Wall -std=c99 -pedantic -Wno-unused-result
CC = gcc

###########################################################################

ep4: ep4.o queue.o
	$(CC) ep4.o queue.o  -o $@

queue.o: queue.c queue.h item.h
	$(CC) $(CFLAGS) -c $<

ep4.o: ep4.c queue.h
	$(CC) $(CFLAGS) -c $<