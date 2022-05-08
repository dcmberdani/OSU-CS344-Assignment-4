CC=gcc --std=gnu99 -g

all: line_processor

mthreads.o: mthreads.c mthreads.h
	$(CC) -c mthreads.c

line_processor: main.c mthreads.o
	$(CC) mthreads.o main.c -o line_processor

clean:
	rm -rf mthreads.o line_processor

run:
	./line_processor
