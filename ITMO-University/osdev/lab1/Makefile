.PHONY: all clean run pack

NAME=lab11vsaN32471
CC=gcc
CFLAGS=-Wall -Wextra -Werror

all: $(NAME)

clean: 
	rm -rf $(NAME)
	rm -rf $(NAME).tar.gz

run: $(NAME)
	./$(NAME)
	
pack: 
	tar -czvf $(NAME).tar.gz *
	
$(NAME): src.c
	$(CC) -o $(NAME) $^ $(CFLAGS)
	

