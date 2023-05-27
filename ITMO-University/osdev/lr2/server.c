#define _DEFAULT_SOURCE // for daemon()
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
#include <signal.h>
#define MSG_SIZE 1024
/*		LAB2: Server part
// 	Tasks: using TCP and pthread make a server to evaluate next requests:
// -ADD num (num is float or integer) - add value to a set
// -GET - return random value from set
// lines end with LF symbol (\n)
// Additional: by default server supports the same set for every client. You have
// to make a launch mode with separate set for each client (identified by IP of sender)
// Response:
// OK - if ADD was g_successfull
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
// change SIGINT, SIGTERM and SIGQUIT to use atomic global variable to stop accepting new threads and finish existing ones
// that will fix valgrind memory issues with Set malloc'd and will be a bit better
//
// misc: check if i received the whole message?

// structs
typedef struct
{
	int clientSocket;
	size_t sleep_time;
	char *ip;
} socketinfo;

typedef struct
{
	double *arr;
	size_t len;
	char *ip;
	// or make a hashmap of sets?
} set_t;

// stats and global vars:
volatile sig_atomic_t g_server_close = 0; //signals set this
size_t g_successful = 0;
size_t g_errors = 0;
int serverSocket = 0;
set_t *Set = &(set_t){NULL, 0, NULL};
int g_output = 0;
int g_separate_flag = 0;
size_t g_set_cnt = 0; // only used if separate_flag is set
pthread_mutex_t set_mutex = PTHREAD_MUTEX_INITIALIZER;
struct timespec start, finish;
volatile sig_atomic_t g_open_threads = 0;

// function prototypes:
static void SIG_handler(int signum);
void *handle_request(void *arg);
void print_datetime();
void check(int t, char *cmd);
char *set_add(double, char *);
char *set_get(char *);
void help(FILE *);

int main(int argc, char **argv)
{
	// time for stats
	check(clock_gettime(CLOCK_MONOTONIC, &start), "clock_gettime() failed!");
	// signals
	struct sigaction sa = {0};
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sigaddset(&sa.sa_mask, SIGQUIT);
	sigaddset(&sa.sa_mask, SIGTERM);
	sa.sa_handler = SIG_handler;
	sa.sa_flags |= (SA_RESTART); // restarts the functions that stopped from interrupts
	check(sigaction(SIGINT, &sa, NULL), "sigaction(SIGINT) failed!");
	check(sigaction(SIGQUIT, &sa, NULL), "sigaction(SIGQUIT) failed!");
	check(sigaction(SIGTERM, &sa, NULL), "sigaction(SIGTERM) failed!");
	check(sigaction(SIGUSR1, &sa, NULL), "sigaction(SIGUSR1) failed!");
	// either use TEMP_FAILURE_RETRY or SA_RESTART

	// check env variables and options:
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

	//getopt loop
	int c;
	while ((c = getopt(argc, argv, "w:dl:a:p:vho")) != -1)
	{
		switch (c)
		{
		case 'w':
			wait_time = strtol(optarg, NULL, 10);
			if (errno != 0)
			{
				print_datetime(stderr);
				fprintf(stderr, "Error: failed to convert -w arg to long int (%s).\nUsing default value (0)...\n", strerror(errno));
				wait_time = 0;
				errno = 0;
			}
			else
			{
				print_datetime(stdout);
				printf("New wait time: %ld\n", wait_time);
			}
			break;
		case 'd':
			int temp = daemon(0, 0);
			if (temp < 0)
			{
				print_datetime(stderr);
				fprintf(stderr, "Failed to open in daemon mode! Continuing in normal mode...\n");
			}
			break;
		case 'l':
			logpath = optarg;
			print_datetime(stdout);
			printf("New log path:%s\n", logpath);
			// TODO: add checking of the value
			break;
		case 'a':
			ip = optarg;
			print_datetime(stdout);
			printf("New IP: %s\n", ip);
			break;
		case 'p':
			port = optarg;
			print_datetime(stdout);
			printf("New port:%s\n", port);
			break;
		case 'v':
			printf("lab2server: Version 0.9\nAuthor: Volkov S.A. N32471\n");
			exit(EXIT_SUCCESS);
			break;
		case 'h':
			help(stdout);
			exit(EXIT_SUCCESS);
			break;
		case 'o':
			g_output = 1;
			break;
		case 's':
			g_separate_flag = 1;
			break;
		default:
			printf("Failed to parse short options!\n");
			break;
		}
	}
	if (getenv("LAB2DEBUG") != NULL)
	{
		print_datetime(stderr);
		fprintf(stderr, "DEBUG:\tVariable values:\nWait time:%ld\nLog path:%s\nIP:%s\nPort:%s\n", wait_time, logpath, ip, port);
	}
	if (g_output)
	{
		if (freopen(logpath, "w", stdout) == NULL)
			perror("freopen() stdout failed: ");
		if (freopen(logpath, "w", stderr) == NULL)
			perror("freopen() stderr failed: ");
	}
	// random seed for set_get()
	srand(time(NULL));

	// opening server socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
	{
		print_datetime(stderr);
		fprintf(stderr, "ERROR: Socket creation failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	uint16_t porti = (uint16_t)strtol(port, NULL, 10);
	struct sockaddr_in server = {0};
	struct sockaddr_storage serverStorage = {0};
	socklen_t addr_size = {0};
	server.sin_family = AF_INET;
	server.sin_port = htons(porti);
	server.sin_addr.s_addr = inet_addr(ip);
	int t = 0;

	t = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	if (t < 0)
	{
		print_datetime(stderr);
		fprintf(stderr, "ERROR: Set sock opt failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	t = bind(serverSocket, (struct sockaddr *)&server, sizeof(server));
	if (t < 0)
	{
		print_datetime(stderr);
		fprintf(stderr, "ERROR: Socket bind failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	setvbuf(stdout, NULL, _IONBF, 0); // to disable buffering (can cause troubles with threads)

	while (g_server_close == 0)
	{
		t = listen(serverSocket, 10);
		if (t < 0)
		{
			print_datetime(stderr);
			fprintf(stderr, "ERROR: Socket listen failed: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		int clientSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size);
		if(clientSocket > 0){

		pthread_t tid;
		addr_size = sizeof(serverStorage);
		struct sockaddr_in *clientIP = (struct sockaddr_in *)&serverStorage;
		socketinfo *args = malloc(sizeof(socketinfo));
		args->clientSocket = clientSocket;
		args->sleep_time = wait_time;
		args->ip = inet_ntoa(clientIP->sin_addr);
		t = pthread_create(&tid, NULL, handle_request, args);
		if (t < 0)
		{
			print_datetime(stderr);
			perror("pthread_create() failed:");
			errno = 0;
		}
		else{
			g_open_threads++;
			pthread_detach(tid);
		}
		} else if(errno = EINVAL) break;
		else perror("accept() failed");
	}
	
	while(g_open_threads!=0)sleep(1);
	sleep(1); //time for last thread to clean up
	close(serverSocket);
	pthread_mutex_lock(&set_mutex);
	if (g_output)
		fclose(stdout); // freopen()'d, need to close
	if (g_output)
		fclose(stdin);
	if (g_separate_flag)
	{
		for (size_t i = 0; i < g_set_cnt; i++)
		{
			free(Set[i].arr);
		}
		free(Set);
	}
	else free(Set[0].arr);
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
				 "-s - separate sets for different ip clients\n"
				 "-o - output in stdin and stderr instead of log file\n"
				 "LAB2DEBUG env - debug mode\n");
}

char *set_get(char *client_ip)
{
	// add verification by IP if necessary
	// i think it can be done using hashmap?
	char tmp[MSG_SIZE];
	if (g_separate_flag)
	{
		if (g_set_cnt == 0)
		{
			sprintf(tmp, "ERROR %d\n", ENODATA);
		}
		else
		{
			int t = 0;
			pthread_mutex_lock(&set_mutex); // mutexes are probably necessary - what if another thread adds a set while we are looking at it?
			for (size_t i = 0; i < g_set_cnt; i++)
			{
				if (strcmp(Set[i].ip, client_ip) == 0)
				{
					if (Set[i].len == 0)
					{
						t++;
						pthread_mutex_unlock(&set_mutex);
						sprintf(tmp, "ERROR %d\n", ENODATA); // i dont know how this might happen, but just in case???
						break;
					}
					else
					{
						sprintf(tmp, "%f\n", Set[0].arr[rand() % Set[0].len]);
						pthread_mutex_unlock(&set_mutex);
						t++;
						break;
					}
				}
			}
			if (tmp == 0)
			{
				pthread_mutex_unlock(&set_mutex);
				sprintf(tmp, "ERROR %d\n", ENODATA);
			}
		}
	}
	else
	{
		if (Set[0].len == 0)
		{
			sprintf(tmp, "ERROR %d\n", ENODATA);
		}
		else
		{
			pthread_mutex_lock(&set_mutex);
			sprintf(tmp, "%f\n", Set[0].arr[rand() % Set[0].len]);
			pthread_mutex_unlock(&set_mutex);
		}
	}
	return strdup(tmp);
}
char *set_add(double value, char *client_ip)
{
	char tmp[MSG_SIZE];
	pthread_mutex_lock(&set_mutex);
	if (g_separate_flag)
	{
		if (g_set_cnt == 0)
		{ // first client ever for add
			Set[0].ip = client_ip;
			g_set_cnt++;
			Set[0].arr = realloc(Set[0].arr, sizeof(double) * (++Set[0].len));
			Set[0].arr[Set[0].len - 1] = value;
			sprintf(tmp, "OK\n");
		}
		else
		{
			short found_flag = 0;
			for (size_t i = 0; i < g_set_cnt; i++)
			{
				if (strcmp(Set[i].ip, client_ip) == 0)
				{
					found_flag++;
					short found = 0;
					for (size_t j = 0; j < Set[i].len; j++)
					{
						if (Set[i].arr[j] == value)
							found++;
					}
					if (found == 0)
					{
						Set[i].arr = realloc(Set[i].arr, sizeof(double) * (++Set[i].len));
						Set[i].arr[Set[i].len - 1] = value;
						sprintf(tmp, "OK\n");
					}
					else
						sprintf(tmp, "ERROR %d\n", EINVAL);
				}
			}
			if (found_flag == 0)
			{
				Set = realloc(Set, sizeof(Set) * (++g_set_cnt));
				Set[g_set_cnt - 1].ip = client_ip;
				Set[g_set_cnt - 1].arr = realloc(Set[g_set_cnt - 1].arr, sizeof(double) * (++Set[g_set_cnt - 1].len));
				Set[g_set_cnt - 1].arr[Set[g_set_cnt - 1].len - 1] = value;
				sprintf(tmp, "OK\n");
			}
		}
	}
	else
	{
		short found = 0;
		for (size_t i = 0; i < Set[0].len; i++)
		{
			if (Set[0].arr[i] == value)
				found++;
		}
		if (found == 0 || Set[0].len == 0)
		{
			Set[0].arr = realloc(Set[0].arr, sizeof(double) * (++Set[0].len));
			Set[0].arr[Set[0].len - 1] = value;
			sprintf(tmp, "OK\n");
		}
		else // if found > 0
		{
			sprintf(tmp, "ERROR %d\n", EINVAL);
		}
	}
	pthread_mutex_unlock(&set_mutex);
	return strdup(tmp);
}

void *handle_request(void *ptr)
{
	socketinfo *arg = ptr;

	char buffer[MSG_SIZE];
	int t = recv(arg->clientSocket, buffer, MSG_SIZE, 0);
	if (t > 0)
	{
		print_datetime(stdout);
		printf("Received:%s\n", buffer); // remove later???

	sleep(arg->sleep_time);
	if (strncmp("ADD", buffer, 3) == 0)
	{
		char *str_end;
		double val = strtod(buffer + 3, &str_end);
		// TODO: Add checking if val is converted correctly (using str_end)
		if (errno == ERANGE)
		{
			print_datetime(stderr);
			fprintf(stderr, "Error: %s\n", strerror(errno));
			g_errors++;
			errno = 0;
		}
		else if (str_end == (buffer + 3))
		{
			print_datetime(stderr);
			fprintf(stderr, "Failed to convert value...\n"); // EINVAL == 22
			t = send(arg->clientSocket, "ERROR 22\n\0", 10, 0);
			check(t, "send() failed:");
			g_errors++;
		}
		else
		{
			char *ans = set_add(val, arg->ip);
			print_datetime(stdout);
			printf("writing:%s", ans); // comment later
			t = send(arg->clientSocket, ans, strlen(ans) + 1, 0);
			if (ans[0] == 'O' && ans[1] == 'K')
				g_successful++;
			else
				g_errors++;
			check(t, "send() failed:");
			free(ans);
		}
	}
	else if (strncmp("GET", buffer, 3) == 0)
	{
		char *ans = set_get(arg->ip);
		print_datetime(stdout);
		printf("Writing:%s", ans); // comment later
		t = send(arg->clientSocket, ans, strlen(ans) + 1, 0);
		check(t, "send() failed:");
		if (ans[0] == 'E')
			g_errors++;
		else
			g_successful++;
		free(ans);
	}
	else if (strncmp("CLOSE", buffer, 5) == 0)
	{
		t = send(arg->clientSocket, "Closing server...\n",19, 0);
		check(t, "send() failed");
		t = raise(SIGINT);
		if (t != 0)
		{
			perror("raise(SIGINT) failed!");
		}
	}
	else
	{
		char ans[MSG_SIZE];
		// cause i want ans to be modifiable? might find a way to make it more pretty
		sprintf(ans, "ERROR %d\n", EINVAL);
		t = send(arg->clientSocket, ans, strlen(ans) + 1, 0);
		check(t, "send() failed:");
	}

	// t = write(clientSocket, buffer, 1024);
	// if(t<0){
	//	fprintf(stderr, "ERROR: Sending failed!: %s\n", strerror(errno));
	//	exit(EXIT_FAILURE);
	// }
	} else perror("recv() failed");

	close(arg->clientSocket);
	free(ptr);
	g_open_threads--;
	pthread_exit(NULL);
	return NULL;
}
void check(int t, char *cmd)
{
	if (t < 0)
	{
		print_datetime(stderr);
		perror(cmd);
	}
}
void print_datetime(FILE *out)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	fprintf(out, "[%02d-%02d-%d] %02d:%02d:%02d  ", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
}
void SIG_handler(int signum)
{
	int err;
	switch (signum)
	{
	case SIGINT:
		err = write(STDOUT_FILENO, "SIGINT received. Closing...\n", 29);
		if (g_output)
			err = write(STDERR_FILENO, "SIGINT received. Closing...\n", 29);
		g_server_close=1;
		err = shutdown(serverSocket, SHUT_RD);
		break;
	case SIGTERM:
		err = write(STDOUT_FILENO, "SIGTERM received. Closing...\n", 30);
		if (g_output)
			err = write(STDERR_FILENO, "SIGTERM received. Closing...\n", 30);
		g_server_close = 1;
		err = shutdown(serverSocket, SHUT_RD);
		break;
	case SIGQUIT:
		err = write(STDOUT_FILENO, "SIGQUIT received. Closing...\n", 30);
		if (g_output)
			err = write(STDERR_FILENO, "SIGQUIT received. Closing...\n", 30);
		g_server_close=1;
		err = shutdown(serverSocket, SHUT_RD);
		break;
	case SIGUSR1:
		err = write(STDOUT_FILENO, "Caught SIGUSR1! Writing stats...\n", 34);
		if (g_output)
			err = write(STDERR_FILENO, "Caught SIGUSR1! Writing stats...\n", 34);

		// i dont know how to print out date & timestamp using async-signal-safe functions :( its hard to do
		// todo: add error checking here??? or not
		err = clock_gettime(CLOCK_MONOTONIC, &finish);

		size_t elapsed = (finish.tv_sec - start.tv_sec);
		char buf[100];
		int i = 0;
		while (elapsed)
		{
			buf[i] = (elapsed % 10) + '0';
			elapsed /= 10;
			i++;
		}
		buf[i] = '\0';
		err = write(STDOUT_FILENO, "Run time: ", 11);
		if (g_output)
			err = write(STDERR_FILENO, "Run time: ", 11);
		for (int j = strlen(buf) - 1; j >= 0; j--)
		{
			err = write(STDOUT_FILENO, buf + j, 1);
			if (g_output)
				err = write(STDERR_FILENO, buf + j, 1);
		}

		if (g_output)
			err = write(STDERR_FILENO, " seconds.\nsuccessful requests:", 31);
		err = write(STDOUT_FILENO, " seconds.\nsuccessful requests:", 31);
		i = 0;

		size_t tmp = g_successful;
		if (tmp == 0)
		{
			err = write(STDOUT_FILENO, "0\n", 1);
			if (g_output)
				err = write(STDERR_FILENO, "0\n", 1);
		}
		else
		{
			while (tmp)
			{
				buf[i] = (tmp % 10) + '0';
				tmp /= 10;
				i++;
			}
			buf[i] = '\0';
			for (int j = strlen(buf) - 1; j >= 0; j--)
			{
				err = write(STDOUT_FILENO, buf + j, 1);
				if (g_output)
					err = write(STDERR_FILENO, buf + j, 1);
			}
		}
		err = write(STDOUT_FILENO, "\nFailed requests:", 18);
		if (g_output)
			err = write(STDERR_FILENO, "\nFailed requests:", 18);
		i = 0;
		if (g_errors == 0)
		{
			err = write(STDOUT_FILENO, "0\n", 2);
			if (g_output)
				err = write(STDERR_FILENO, "0\n", 2);
		}
		else
		{
			tmp = g_errors;
			while (tmp)
			{
				buf[i] = (tmp % 10) + '0';
				tmp /= 10;
				i++;
			}
			buf[i] = '\0';
			for (int j = strlen(buf) - 1; j >= 0; j--)
			{
				err = write(STDOUT_FILENO, buf + j, 1);
				if (g_output)
					err = write(STDERR_FILENO, buf + j, 1);
			}
			err = write(STDOUT_FILENO, "\n", 1);
			if (g_output)
				err = write(STDERR_FILENO, "\n", 1);
			if (err != 0)
				err = write(STDERR_FILENO, "Error while writing!", 21); // required to compile with -O3 to ignore unused result...
			// if write gives an error we must write that it had errors, and that write might have errors......
		}
		break;
	}
}
