SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJ = $(SRC:.c=.o)
OBJ := $(OBJ:$(SRC_DIR)/%=%)
OBJ := $(addprefix $(OBJ_DIR)/, $(OBJ))

CC = gcc
LD = gcc

CFLAGS = -Wall -Wextra -I. -fPIC -g
LDFLAGS = -shared

LIBS = 

NAME = libcheese
NAME_SO = $(NAME).so
NAME_A = $(NAME).a

all: $(NAME_SO) $(NAME_A)

$(NAME_SO): $(OBJ)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

$(NAME_A): $(OBJ)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME_A) $(NAME_SO)

re: fclean all

.PHONY: re fclean clean all
