#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/*		LAB2 Client.
//
//
//
//
*/
int g_server_closed = 0;
void help();

int main(int argc, char **argv)
{
	char *ip = (getenv("LAB2ADDR") != NULL) ? getenv("LAB2ADDR") : "127.0.0.1";
	char *port = (getenv("LAB2PORT") != NULL) ? getenv("LAB2PORT") : "8008";

	int c;
	double add = 0;
	int testv = 0;
	int get = 0;
	while ((c = getopt(argc, argv, "a:p:vhgd:")) != -1)
	{
		switch (c)
		{
		case 'a':
			ip = optarg;
			printf("New IP: %s\n", ip);
			break;
		case 'p':
			port = optarg;
			printf("New port:%s\n", port);
			break;
		case 'v':
			printf("lab2client: Version 0.5\nAuthor: Volkov S.A. N32471\n");
			exit(EXIT_SUCCESS);
			break;
		case 'h':
			help();
			exit(EXIT_SUCCESS);
			break;
		case 'd': // ADD request
			if (testv == 1)
			{
				fprintf(stderr, "Only 1 test request per launch!\n");
				exit(EXIT_FAILURE);
			}
			add = strtod(optarg, NULL);
			testv = 1;
			break;
		case 'g': // ADD request
			if (testv == 1)
			{
				fprintf(stderr, "Only 1 test request per launch!\n");
				exit(EXIT_FAILURE);
			}
			get = 1;
			testv = 1;
			break;
		default:
			printf("Failed to parse short options!\n");
			break;
		}
	}
	int clientSocket;
	struct sockaddr_in serverAddr = {0};
	// socklen_t addr_size;

	uint16_t porti = (uint16_t)strtol(port, NULL, 10);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(porti);
	serverAddr.sin_addr.s_addr = inet_addr(ip);
	if (getenv("LAB2DEBUG") != NULL)
	{
		fprintf(stderr, "DEBUG:\t\tConnecting to:\n IP:%s\nPort:%s\n", ip, port);
	}
	while (g_server_closed == 0)
	{
		clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (clientSocket < 0)
		{
			fprintf(stderr, "Failed to create client socket! %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		int t = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
		if (t < 0)
		{
			fprintf(stderr, "Failed to connect socket! %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		char buffer[1024];
		if (testv == 1)
		{
			if (get == 1)
				sprintf(buffer, "GET");
			else
				sprintf(buffer, "ADD %f", add);
			t = send(clientSocket, buffer, 1024, 0);
			if (t < 0)
			{
				fprintf(stderr, "Failed to send!%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			char buff2[1024];
			t = recv(clientSocket, buff2, 1024, 0);
			if (t < 0)
			{
				fprintf(stderr, "Failed to receive!%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			printf("Data received (%d bytes): %s", (int)t, buff2);
			close(clientSocket);
			exit(EXIT_SUCCESS);
		}
		else if (fgets(buffer, 1024, stdin) != NULL)
		{
			if (strncmp(buffer, "EXIT", 4) == 0)
			{
				close(clientSocket);
				break;
			}
			else if (strncmp(buffer, "CLOSE", 5) == 0){
				g_server_closed++;
			}
			
			if (buffer[strlen(buffer) - 1] != '\n')
			{
				fprintf(stderr, "MSG too long! Max message size:%d\n", 1024);
				continue;
			}

			t = send(clientSocket, buffer, 1024, 0);
			if (t < 0)
			{
				fprintf(stderr, "Failed to send!%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			char buff2[1024];
			t = recv(clientSocket, buff2, 1024, 0);
			if (t < 0)
			{
				fprintf(stderr, "Failed to receive!%s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
			printf("Data received (%d bytes): %s", (int)t, buff2);
		}
		else
			fprintf(stderr, "Failed to gets from stdin!\n");

		close(clientSocket);
	}
	return 0;
}

void help()
{
	printf("\t\tOptions:\n");
	printf("-a xxx.xxx.xxx.xxx (or LAB2ADDR) - listening ip address - default [127.0.0.1]\n");
	printf("-p port (or LAB2PORT) - listening port - default [8008]\n");
	printf("-v - version\n");
	printf("-h - help\n");
	printf("-g - send GET request to server and close client (test mode)");
	printf("-d [value] - send ADD [value] request to server and close client (test mode)");
	printf("LAB2DEBUG env - debug mode\n");
	printf("\t\tAvailable commands to send to server:\n"
		   "ADD [value] - add value to the set\n"
		   "GET - get random value from the set\n"
		   "CLOSE - close server\n"
		   "EXIT - close client\n");
}
