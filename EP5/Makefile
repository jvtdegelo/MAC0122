CFLAGS= -g -Wall -g -O0 -std=c99 -pedantic -Wno-unused-result
CC = gcc
RM = rm
#-------------------------------------------------

imdb:  main.o  util.o iofilmes.o filmes.o st.o 
	$(CC) main.o util.o iofilmes.o filmes.o st.o -o imdb

main.o: main.c filmes.h util.h iofilmes.h
	$(CC) $(CFLAGS) -c main.c

st.o    : st.c filmes.h util.h st.h
	$(CC) $(CFLAGS) -c st.c 

filmes.o: filmes.c filmes.h util.h iofilmes.h
	$(CC) $(CFLAGS) -c filmes.c

iofilmes.o: iofilmes.h iofilmes.c
	$(CC) $(CFLAGS) -c iofilmes.c

util.o: util.h util.c
	$(CC) $(CFLAGS) -c util.c

clean: 
	$(RM) *.o *~ imdb
