NAME = md5_program
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iincludes
SRC = src/main.c src/md5.c src/debug.c
OBJ_DIR = objs
OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
