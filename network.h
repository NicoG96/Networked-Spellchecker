/* LIBRARIES */
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>


/* MACROS */
#define SOCKET_DEFAULT 80
#define DICT_DEFAULT "dictionary.txt"


/* GLOBAL VARS */
FILE *DICTIONARY;
int SOCKET;


/* STRUCTS */
typedef struct client{
    int sock_desc;
    struct client *next;
}client;

/* FUNCTIONS */
