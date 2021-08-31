
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L 
all:build

build:
	gcc $(CFLAGS) s-talk.c list.c -lpthread -o s-talk

run:build
	./output

valgrind:build	
	valgrind --leak-check=full ./output

clean:
	rm -f output