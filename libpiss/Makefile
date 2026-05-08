SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

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

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME_A) $(NAME_SO)

re: fclean all

.PHONY: re fclean clean all
