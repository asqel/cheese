SRC = $(wildcard */)
OBJ = $(SRC:/=.so) 

INC = -I../include/ -I../oeuf/ -I../onion/include
CFLAGS = -Wall -Wextra $(INC) -fPIC
CC = gcc

all: $(OBJ)

%.so: %/
	$(CC) -shared $(CFLAGS) -c $</*.c
	@mkdir -p $</obj/
	@mv *.o $</obj/ 
	$(CC) -o $@ -shared $(CFLAGS) $</obj/*.o

fclean:
	rm -rf $(wildcard */obj/)
