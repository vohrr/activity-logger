CC=gcc #compiler

CFLAGS=-Wall -Wextra -pedantic -std=c99 -g 

PKG=$(shell pkg-config --libs gtk4)
PKG_FLAGS=$(shell pkg-config --cflags gtk4 | sed 's/-I/-isystem /g')
SRC=src/*.c 

OUT=activity_logger

$(OUT) : $(SRC)
	$(CC) $(CFLAGS) $(PKG_FLAGS) $(SRC) -o $(OUT) $(PKG)  

clean:
	rm -f $(OUT)
