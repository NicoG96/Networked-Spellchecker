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
#define WORKER_BUF 1
#define CLIENT_BUF 1


/* GLOBAL VARS */
FILE *DICTIONARY;
FILE *LOG;
int LISTEN_PORT;


/* STRUCTS */
typedef struct client{
    int sock_desc;
    struct client *next;
}client;


/* FUNCTIONS */
void enqueue(client *newclient, client **queue);
client* pop(client **queue);
_Bool isempty(client **queue);
_Bool isfull(client **queue);
size_t size(client **queue);
void print(client **queue);
client* newNode();
void* threadFunction(void* id);
_Bool lookup(char *word);
void *worker();
void *logger();
int open_listenfd(int port);