SRC = $(wildcard src/*.c src/*/*.c)
OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:src/%=%)
OBJ := $(addprefix obj/, $(OBJ))

CC = gcc
LD = gcc

LIBS_INCLUDE = -Ioeuf/ -Ionion/include
LIBS = liboeuf.a libonion.a
CFLAGS = -Wall -Wextra -Iinclude $(LIBS_INCLUDE) -g -rdynamic
LDFLAGS = -rdynamic -Wl,--no-as-needed -ldl -lpthread

NAME = cheese

all: $(NAME)
	make -C rooms

$(NAME): $(OBJ) $(LIBS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)
	make -C onion clean
	make -C oeuf clean
	make -C rooms fclean

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
