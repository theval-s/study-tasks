.PHONY: all clean pack

NAME=lab1vsaN32471
LNAME=libvsaN32471
CC=gcc
CFLAGS=-Wall -Wextra -Werror

all: $(NAME) libvsaN32471.so

clean: 
	rm -rf $(NAME) $(NAME).tar.gz *.o $(LNAME).so
	
pack: 
	tar -czvf $(NAME).tar.gz *
	
$(NAME): src.c utils.o
	$(CC) -o $(NAME) $^ $(CFLAGS)
	
utils.o: utils.c
	$(CC) $^ $(CFLAGS) -c

$(LNAME).so: $(LNAME).c
	$(CC) -shared -fPIC $^ -o $@ -ldl -lm $(CFLAGS)
	

