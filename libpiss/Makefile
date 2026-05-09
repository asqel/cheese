SRC = $(wildcard src/*.c src/*/*.c)
OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:src/%=%)
OBJ := $(addprefix obj/, $(OBJ))

CC = gcc
LD = gcc

CFLAGS = -Wall -Wextra -I. -fPIC -g
LDFLAGS = -shared

NAME = libpiss
NAME_SO = $(NAME).so
NAME_A = $(NAME).a

all: $(NAME_SO) $(NAME_A)

$(NAME_SO): $(OBJ)
	$(LD) $(LDFLAGS) $^ ../libcheese/libcheese.a -o $@

$(NAME_A): $(OBJ)
	ar rcs $@ $^

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj/

fclean: clean
	rm -rf $(NAME_A) $(NAME_SO)

re: fclean all

.PHONY: re fclean clean all
