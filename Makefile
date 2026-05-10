SRC = $(wildcard src/*.c src/*/*.c)
OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:src/%=%)
OBJ := $(addprefix obj/, $(OBJ))

CC = gcc
LD = gcc

LIBS_INCLUDE = -Ioeuf/ -Ionion/include -Ilibcheese/
LIBS = liboeuf.a libonion.a libcheese.a libpiss.a
LIBS_DIR_NAME = libs/
LIBS_DIR = $(addprefix $(LIBS_DIR_NAME), $(LIBS))

CFLAGS = -Wall -Wextra -Iinclude $(LIBS_INCLUDE) -g -rdynamic
LDFLAGS = -rdynamic -Wl,--no-as-needed -ldl -lpthread

NAME = cheese

all: $(NAME)
	make -C rooms

$(NAME): $(OBJ) $(LIBS)
	mkdir -p $(LIBS_DIR_NAME)
	mv $(LIBS) $(LIBS_DIR_NAME)
	$(LD) $(LDFLAGS) $(OBJ) $(LIBS_DIR) -o $@

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj/
	make -C onion clean
	make -C oeuf clean
	make -C rooms fclean
	make -C libpiss fclean
	make -C libcheese fclean

fclean: clean
	rm -rf $(NAME)
	rm -rf $(LIBS_DIR_NAME)

liboeuf.a:
	@mkdir -p $(LIBS_DIR_NAME)
	make -C oeuf re
	cp oeuf/$@ ./

libonion.a:
	@mkdir -p $(LIBS_DIR_NAME)
	make -C onion re
	cp onion/$@ ./

libcheese.a:
	@mkdir -p $(LIBS_DIR_NAME)
	make -C libcheese re
	cp libcheese/$@ ./

libpiss.a:
	@mkdir -p $(LIBS_DIR_NAME)
	make -C libpiss re
	cp libpiss/$@ ./
	cp libpiss/libpiss.so $(LIBS_DIR_NAME)

re: fclean all

.PHONY: re fclean clean all
