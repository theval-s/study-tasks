#include <errno.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h> //getenv and strtol
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

/*		LAB2: Server part
// 	Tasks: using TCP and pthread make a server to evaluate next requests:
// -ADD num (num is float or integer) - add value to a set
// -GET - return random value from set
// lines end with LF symbol (\n)
// Additional: by default server supports the same set for every client. You have
// to make a launch mode with separate set for each client (identified by IP of sender)
// Response:
// OK - if ADD was successfull
// random number from set - for GET
// ERROR N\n - if there is an error. N is a error number
//////////////////////////////////////////////////////////
//		Options:
// -w N (or LAB2WAIT env) - imitation of hard work by freezing the thread for N seconds - 0 by default
// -d - daemon mode
// -l /path/ (or LAB2LOGFILE env) - path to log file - default - /tmp/lab2.log
// -a ip (or LAB2ADDR) - listening address
// -p port (or LAB2PORT) - listening port
// -v - version
// -h - help
// LAB2DEBUG env - debug mode
*/
void help(FILE *);

int main(int argc, char **argv)
{
	size_t wait_time = (getenv("LAB2WAIT") != NULL) ? strtol(getenv("LAB2WAIT"), NULL, 10) : 0;
	if(errno != 0){
		fprintf(stderr, "Error: LAB2WAIT not NULL, but couldn't parse value (errno:%s).\n Using default value...\n", strerror(errno));
		wait_time = 0;
		errno = 0;
	}
	char *logpath = (getenv("LAB2LOGFILE") != NULL) ? getenv("LAB2LOGFILE") : "/tmp/lab2.log";
	char *ip = (getenv("LAB2ADDR") != NULL) ? getenv("LAB2ADDR") : "127.0.0.1";
	char *port = (getenv("LAB2PORT") != NULL) ? getenv("LAB2PORT") : "8008";
	FILE *outs = stdout;
	//^ can change to log file when needed! 
	int c;
	while ((c = getopt(argc, argv, "w:dl:a:p:vh")) != -1)
	{
		switch (c)
		{
		case 'w':
			wait_time = strtol(optarg, NULL, 10);
			if (errno != 0)
			{
				fprintf(stderr, "Error: failed to convert -w arg to long int (%s).\nUsing default value (0)...\n", strerror(errno));
				wait_time = 0;
				errno = 0;
			}
			else
				fprintf(outs, "New wait time: %ld\n", wait_time);
			break;
		case 'd':
			// TODO: how to launch in daemon mode?
			break;
		case 'l':
			logpath = optarg;
			fprintf(outs, "New log path:%s\n", logpath);
			// TODO: add checking of the value
			// thoughts: just fopen and if it fails use default value instead
			break;
		case 'a':
			ip = optarg;
			fprintf(outs, "New IP: %s\n", ip);
			break;
		case 'p':
			port = optarg;
			fprintf(outs, "New port:%s\n", port);
			break;
		case 'v':
			fprintf(outs,"lab2server: Version 0.07\nAuthor: Volkov S.A. N32471\n");
			exit(EXIT_SUCCESS);
			break;
		case 'h':
			help(outs);
			exit(EXIT_SUCCESS);
			break;
		default:
			fprintf(outs, "Failed to parse short options!\n");
			break;
		}
	}
	if(getenv("LAB2DEBUG")!=NULL){
		fprintf(stderr, "DEBUG:\tVariable values:\nWait time:%ld\nLog path:%s\nIP:%s\nPort:%s\n", wait_time, logpath, ip, port);
	}
	//getopt and env done - now for the main part
	srand(time(NULL));
	//for GET^ might want to move somewhere else for better randomness??
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket < 0){
		fprintf(stderr, "ERROR: Socket creation failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	uint16_t porti = (uint16_t)strtol(port, NULL, 10);
	struct sockaddr_in server = {0};
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;	
	server.sin_family = AF_INET;
	server.sin_port = htons(porti);
	server.sin_addr.s_addr = inet_addr(ip);
	int t = 0;
	t = bind(serverSocket, (struct sockaddr *)&server, sizeof(server));
	if(t < 0){
		fprintf(stderr, "ERROR: Socket bind failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	while(1){
		t = listen(serverSocket, 10);
		if(t < 0){
			fprintf(stderr, "ERROR: Socket listen failed: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}	
		addr_size = sizeof(serverStorage);
		int clientSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size);
		if(clientSocket < 0){
			fprintf(stderr, "ERROR: Client socket accept failed!: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		char buffer[1024];
		t = read(clientSocket, buffer, 1024);
		if(t>0) printf("Received!!!%s\n", buffer);
		if(t<0){
			fprintf(stderr, "ERROR: Receive failed!: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		t = write(clientSocket, buffer, 1024);
		if(t<0){
			fprintf(stderr, "ERROR: Sending failed!: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		close(clientSocket);
	}
	close(serverSocket);
	return 0;
}

void help(FILE *out)
{
	fprintf(out, "\t\tOptions:\n");
	fprintf(out, "-w N (or LAB2WAIT) - imitation of hard work by freezing the thread for N seconds - default [0]\n");
	fprintf(out, "-d - daemon mode\n");
	fprintf(out, "-l /path/ (or LAB2LOGFILE) - path to log file - default [/tmp/lab2.log]\n");
	fprintf(out, "-a xxx.xxx.xxx.xxx (or LAB2ADDR) - listening ip address - default [127.0.0.1]\n");
	fprintf(out, "-p port (or LAB2PORT) - listening port - default [8008]\n");
	fprintf(out, "-v - version\n");
	fprintf(out, "-h - help\n");
	fprintf(out, "LAB2DEBUG env - debug mode\n");
}