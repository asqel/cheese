SRC = $(wildcard */)
OBJ = $(SRC:/=.so) 

INC = -I../include/ -I../oeuf/ -I../onion/include
CFLAGS = -Wall -Wextra $(INC)
CC = gcc

all: $(OBJ)

%.so: %/
	$(CC) -o $@ -shared $(CFLAGS) $</*.c

fclean:
	rm -rf $(OBJ)
