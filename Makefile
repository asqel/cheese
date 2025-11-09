SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

CC = gcc
LD = gcc

CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = 

NAME = cheese

all: $(NAME)

$(NAME): $(OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: re fclean clean all
