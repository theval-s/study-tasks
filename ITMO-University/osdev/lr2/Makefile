.PHONY: all clean pack
CFLAGS=-Wall -Wextra -Werror
all: server client

clean:
	rm -rf *.o server client lab2vsaN32471.tar.gz
server: server.c
	gcc -o server $^ $(CFLAGS)
client: client.c
	gcc -o client $^ $(CFLAGS)
pack:
	tar -czvf lab2vsaN32471.tar.gz *
