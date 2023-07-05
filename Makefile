NAME = dasm
CFLAGS = -Wall -Wextra -Wshadow
LDFLAGS =

SRC = src/main.c \
	  src/dasm.c \
	  src/instructions.c \
	  src/interpretor.c

OBJ = $(SRC:%.c=%.o)


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
