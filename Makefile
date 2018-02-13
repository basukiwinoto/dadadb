# Makefile for dadadb
CC     = gcc
CFLAGS = -Wall -Wextra -g 

all: dadadb

dadadb: dadadb.o lsmtreelib.o
	$(CC) $(CFLAGS) dadadb.o lsmtreelib.o -o dadadb
	
dadadb.o: dadadb.c
	$(CC) $(CFLAGS) -c dadadb.c

lsmtreelib.o: lsmtreelib.c
	$(CC) $(CFLAGS) -c lsmtreelib.c
	
clean:
	rm -f dadadb *.o

# Test
test: dadadb
	./dadadb sample_workload