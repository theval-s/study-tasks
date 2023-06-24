.PHONY: all clean pack

NAME=lab1vsaN32471
LNAME=libvsaN32471
CC=gcc
CFLAGS=-Wall -Wextra -Werror -g

all: $(NAME) libvsaN32471.so

clean: 
	rm -rf $(NAME) $(NAME).tar.gz *.o $(LNAME).so
	
pack: 
	tar -czvf $(NAME).tar.gz *
	
$(NAME): src.c utils.o
	$(CC) -o $(NAME) $^ $(CFLAGS) -O3 -ldl 
	
utils.o: utils.c
	$(CC) $^ $(CFLAGS) -c -O3

$(LNAME).so: $(LNAME).c
	$(CC) -shared -fPIC $^ -o $@ -ldl -lm $(CFLAGS)
	

