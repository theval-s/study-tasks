.PHONY: all clean pack

NAME=lab1vsaN32471
CC=gcc
CFLAGS=-Wall -Wextra -Werror

all: $(NAME) libvsaN32471

clean: 
	rm -rf $(NAME)
	rm -rf $(NAME).tar.gz
	rm -rf *.
	rm -rf *.so
	
pack: 
	tar -czvf $(NAME).tar.gz *
	
$(NAME): src.c utils.o
	$(CC) -o $(NAME) $^ $(CFLAGS)
	
utils.o: utils.c
	$(CC) $^ $(CFLAGS) -c

libvsaN32471:
	$(CC) -shared -fPIC $@.c -o $@.so -ldl -lm $(CFLAGS)
	

