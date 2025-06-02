CC=gcc #compiler

CFLAGS=-Wall -Wextra -pedantic -std=c11 -g

SRC=src/*.c

OUT=activity_logger

$(OUT) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC)

clean:
	rm -f $(OU)
