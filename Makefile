SRC = $(wildcard src/*.c src/*/*.c)
OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:src/%=%)
OBJ := $(addprefix obj/, $(OBJ))

CC = gcc
LD = gcc

LIBS_INCLUDE = -Ioeuf/ -Ionion/include
LIBS = liboeuf.a libonion.a
CFLAGS = -Wall -Wextra -Iinclude $(LIBS_INCLUDE) -g 
LDFLAGS =

NAME = cheese

all: $(NAME)

$(NAME): $(OBJ) $(LIBS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)
	make -C oeuf clean

fclean: clean
	rm -rf $(NAME)
	rm -rf $(LIBS)

liboeuf.a:
	make -C oeuf
	cp oeuf/$@ ./

libonion.a:
	make -C onion
	cp onion/$@ ./

re: fclean all

.PHONY: re fclean clean all
