NAME = md5_program
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iincludes
SRC = src/main.c src/md5.c src/debug.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
