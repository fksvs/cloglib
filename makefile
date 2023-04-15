CC = gcc
CFLAGS = -Wall -lpthread

all : benchmark example

benchmark :
	$(CC) $(CFLAGS) -o test/benchmark src/cloglib.c test/benchmark.c

example :
	$(CC) $(CFLAGS) -o test/example src/cloglib.c test/example.c

.PHONY = clean
clean:
	find test/ -name "benchmark" -delete
	find test/ -name "example" -delete
	find . -name "*.log" -delete
