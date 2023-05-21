.PHONY: all clean pack
CFLAGS=-Wall -Wextra -Werror
all: server client

clean:
	rm -rf *.o server client lab2vsaN32471.tar.gz
server: server.c
	gcc -o server $^ $(CFLAGS) -pthread
client: client.c
	gcc -o client $^ $(CFLAGS)
pack:
	tar -czvf lab2vsaN32471.tar.gz *
test: all 
	 ./test.sh
	sleep 1
	./test2.sh
