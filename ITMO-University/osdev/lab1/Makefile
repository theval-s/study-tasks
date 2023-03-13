.PHONY: all clean run

NAME=lab11vsaN32471 
CC=gcc
CFLAGS=-Wall -Wextra -Werror
all: $(NAME)

clean: 
	rm -rf *.o 
	rm -rf $(NAME) 

run: $(NAME)
	./$(NAME)
	
$(NAME): src.c
	$(CC) -o $(NAME) $^ $(CFLAGS)
	

