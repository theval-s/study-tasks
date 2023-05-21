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
#define MSG_SIZE 1024
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

// TODO:
//	options and env variables -- done
//	make socket connection -- done
//	threading of socket connectivity -- kind of? some errors
//	add set data structure -- kind of done, might add sort?
//	work with signals
//	log file
// misc: check if i received the whole message?
void help(FILE *);
typedef struct
{
	int serverSocket;
	int clientSocket;
	size_t sleep_time;
} socketinfo;

typedef struct
{
	double *arr;
	size_t len;
	// char ip can be added to identify? need to change init that way
} set_t;

set_t Set = {NULL, 0};

char *set_get();	   // question: can get be done as non-blocking op if we pass a copy at the moment of the call? will that be correct?
char *set_add(double); // return OK if everything is correct

void *handle_request(void *arg);

pthread_mutex_t set_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	size_t wait_time = (getenv("LAB2WAIT") != NULL) ? strtol(getenv("LAB2WAIT"), NULL, 10) : 0;
	if (errno != 0)
	{
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
			fprintf(outs, "lab2server: Version 0.3\nAuthor: Volkov S.A. N32471\n");
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
	if (getenv("LAB2DEBUG") != NULL)
	{
		fprintf(stderr, "DEBUG:\tVariable values:\nWait time:%ld\nLog path:%s\nIP:%s\nPort:%s\n", wait_time, logpath, ip, port);
	}
	// getopt and env done - now for the main part
	srand(time(NULL));
	// for GET^ might want to move somewhere else for better randomness??
	// TODO: add signals catching

	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
	{
		fprintf(stderr, "ERROR: Socket creation failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	uint16_t porti = (uint16_t)strtol(port, NULL, 10);
	struct sockaddr_in server = {0};
	struct sockaddr_storage serverStorage = {0};
	socklen_t addr_size;
	server.sin_family = AF_INET;
	server.sin_port = htons(porti);
	server.sin_addr.s_addr = inet_addr(ip);
	int t = 0;

	t = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	if (t < 0)
	{
		fprintf(stderr, "ERROR: Set sock opt failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	t = bind(serverSocket, (struct sockaddr *)&server, sizeof(server));
	if (t < 0)
	{
		fprintf(stderr, "ERROR: Socket bind failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	setvbuf(stdout, NULL, _IONBF, 0);
	while (1)
	{
		t = listen(serverSocket, 10);
		if (t < 0)
		{
			fprintf(stderr, "ERROR: Socket listen failed: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		int clientSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size);
		if (clientSocket < 0)
		{
			fprintf(stderr, "ERROR: Client socket accept failed!: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		pthread_t tid;
		addr_size = sizeof(serverStorage);
		socketinfo args = {serverSocket, clientSocket, wait_time};
		t = pthread_create(&tid, NULL, handle_request, &args);
		if (t < 0)
		{
			fprintf(stderr, "ERROR: Thread creation failed%s\n", strerror(errno));
			errno = 0;
			continue;
		}
		else pthread_detach(t);
	}
	close(serverSocket);
	pthread_mutex_destroy(&set_mutex);
	return 0;
}

void help(FILE *out)
{
	fprintf(out, "\t\tOptions:\n"
				 "-w N (or LAB2WAIT) - imitation of hard work by freezing the thread for N seconds - default [0]\n"
				 "-d - daemon mode\n"
				 "-l /path/ (or LAB2LOGFILE) - path to log file - default [/tmp/lab2.log]\n"
				 "-a xxx.xxx.xxx.xxx (or LAB2ADDR) - listening ip address - default [127.0.0.1]\n"
				 "-p port (or LAB2PORT) - listening port - default [8008]\n"
				 "-v - version\n"
				 "-h - help\n"
				 "LAB2DEBUG env - debug mode\n");
}

char *set_get()
{
	// add verification by IP if necessary
	// i think it can be done using hashmap?
	char tmp[MSG_SIZE];
	if (Set.len == 0)
	{
		sprintf(tmp, "ERROR %d\n", ENODATA);
	}
	else
	{
		pthread_mutex_lock(&set_mutex);
		sprintf(tmp, "%f\n", Set.arr[rand() % Set.len]);
		pthread_mutex_unlock(&set_mutex);
	}
	return strdup(tmp);
}
char *set_add(double value)
{
	// add mutex locking
	// i dont have to sort it, do i????
	pthread_mutex_lock(&set_mutex);
	char tmp[MSG_SIZE];
	short found = 0;
	for (size_t i = 0; i < Set.len; i++)
	{
		if (Set.arr[i] == value)
			found++;
	}
	if (found == 0 || Set.len == 0)
	{
		Set.arr = realloc(Set.arr, sizeof(double) * (++Set.len));
		Set.arr[Set.len - 1] = value;
		sprintf(tmp, "OK\n");
		pthread_mutex_unlock(&set_mutex);
	}
	else
	{
		// if found > 0
		sprintf(tmp, "ERROR %d\n", EPERM);
		pthread_mutex_unlock(&set_mutex);
	}
	return strdup(tmp);
}

void *handle_request(void *ptr)
{
	socketinfo *arg = ptr;

	char buffer[MSG_SIZE];
	int t = recv(arg->clientSocket, buffer, MSG_SIZE, 0);
	if (t > 0)
		printf("Received:%s\n", buffer);
	if (t < 0)
	{
		fprintf(stderr, "ERROR: Receive in thread failed!: %s\n", strerror(errno));
		return NULL;
	}

	sleep(arg->sleep_time);
	if (strncmp("ADD", buffer, 3) == 0)
	{
		char *str_end;
		double val = strtod(buffer + 3, &str_end);
		//TODO: Add checking if val is converted correctly (using str_end)
		char *ans = set_add(val);
		t = send(arg->clientSocket, ans, strlen(ans)+1,0);
		printf("writing:%s\n", ans);
		if (t < 0)
		{
			fprintf(stderr, "ERROR: Sending failed!: %s\n", strerror(errno));
			return NULL;
		}
		free(ans);

		if (errno == ERANGE)
		{
			fprintf(stderr, "Error: %s\n", strerror(errno));
			errno = 0;
		}
	}
	else if (strncmp("GET", buffer, 3) == 0)
	{
		char *ans = set_get();
		printf("writing:%s\n", ans);
		t = send(arg->clientSocket, ans, strlen(ans)+1, 0);
		if (t < 0)
		{
			fprintf(stderr, "ERROR: Sending failed!: %s\n", strerror(errno));
		}
		free(ans);
	}
	else
	{
		char ans[MSG_SIZE];
		// cause i want ans to be modifiable? might find a way to make it more pretty
		sprintf(ans, "ERROR %d\n", EINVAL);
		t = send(arg->clientSocket, ans, strlen(ans)+1, 0);
		if (t < 0)
		{
			fprintf(stderr, "ERROR: Sending failed!: %s\n", strerror(errno));
		}
	}

	// t = write(clientSocket, buffer, 1024);
	// if(t<0){
	//	fprintf(stderr, "ERROR: Sending failed!: %s\n", strerror(errno));
	//	exit(EXIT_FAILURE);
	// }

	close(arg->clientSocket);
	pthread_exit(NULL);
	return NULL;
}