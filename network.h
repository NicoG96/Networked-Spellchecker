/* LIBRARIES */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <pthread.h>


/* MACROS */
#define SOCKET_DEFAULT 8765
#define DICT_DEFAULT "dictionary.txt"
#define DICT_BUF 128
#define BUFFER_MAX 5


/* GLOBAL VARS */
FILE *DICTIONARY;
FILE *LOG;
int LISTEN_PORT;


/* STRUCT */
typedef struct server{
    char **log_buf;
    int *client_buf;
    int client_count, log_count;
    int client_index, log_index;
    pthread_mutex_t client_mutex, log_mutex;
    pthread_cond_t client_not_empty, client_not_full;
    pthread_cond_t log_not_empty, log_not_full;
}server;


/* FUNCTIONS */
_Bool lookup(char *word);
void *worker_routine(void *args);
void *logger_routine(void *args);
int open_listenfd(int port);
void server_init(server *serv);
void insert_client(server *serv, int socket);
int remove_client(server *serv);
void insert_log(server *serv, char *word, int iscorrect);
int remove_log(server *serv, char **logitem);
