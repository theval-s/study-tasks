.PHONY: all clean pack

NAME=lab12vsaN32471
CC=gcc
CFLAGS=-Wall -Wextra -Werror

all: $(NAME)

clean: 
	rm -rf $(NAME)
	rm -rf $(NAME).tar.gz
	rm -rf *.o
	
pack: 
	tar -czvf $(NAME).tar.gz *
	
$(NAME): src.c utils.o
	$(CC) -o $(NAME) $^ $(CFLAGS)
	
utils.o: utils.c
	$(CC) $^ $(CFLAGS) -c
	

