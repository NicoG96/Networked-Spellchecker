#include "network.h"

int main(int argc, char *argv[]) {
    //default dictionary and socket
    if (argc == 1) {
        if (!(DICTIONARY = fopen(DICT_DEFAULT, "r"))) {
            perror("Dictionary");
            exit(EXIT_FAILURE);
        }
        LISTEN_PORT = SOCKET_DEFAULT;
    }

    //passed a dictionary or socket, figure out which
    else if (argc == 2) {
        char *endptr;

        //if no integers in argument, assume its a dictionary file
        if (!(LISTEN_PORT = (int) strtol(argv[1], &endptr, 10))) {
            if (!(DICTIONARY = fopen(argv[1], "r"))) {
                perror("Unknown dictionary");
                exit(EXIT_FAILURE);
            }
            LISTEN_PORT = SOCKET_DEFAULT;
        }
    }

    //passed specific dictionary and socket
    else {
        if (!(DICTIONARY = fopen(argv[1], "r"))) {
            perror("Unknown dictionary");
            exit(EXIT_FAILURE);
        }
        char *endptr;

        if (!(LISTEN_PORT = (int) strtol(argv[2], &endptr, 10))) {
            perror("Issue reading socket");
            exit(EXIT_FAILURE);
        }
    }
    /*
    printf("Dictionary:\t%s\n", argv[1]);
    printf("Socket:\t%d\n", SOCKET);
     */

    //create log file
    LOG = fopen("log.txt", "w+");

    //create log queue
    int logQ[1];

    //create thread pool
    pthread_t workers[WORKER_BUF];

    //fill pool
    pthread_create(&workers[0], NULL, worker(), NULL);

    //create client queue
    int clients[CLIENT_BUF];

    //create a listening socket on the specified port
    int listen_socket;
    if ((listen_socket = open_listenfd(LISTEN_PORT)) < 0) {
        perror("Listening socket");
        return 1;
    }

    //create socket variable for client connections
    int connected_socket;

    //start accepting clients
    while (1) {
        connected_socket = accept(listen_socket, NULL, NULL);

        //get clientsQ lock

        //add client socket to Q
        clients[0] = connected_socket;

        //unlock Q


        //signal workers

        break;
    }
    return 0;
}

_Bool lookup(char *word) {
    //clear stdin
    fflush(stdin);

    _Bool match = 0;
    char buf[DICT_BUF];

    while((fgets(buf, DICT_BUF, DICTIONARY) != NULL)) {
        //printf("%s", buf);

        //get rid of '\n' first
        size_t len = strlen(buf);
        if(buf[len - 1] == '\n' && len > 1) {
            buf[len - 1] = '\0';
        }

        //if the word isn't a match, get next word
        if(strcmp(buf, word) != 0) {
            continue;
        }
        else {
            //printf("\"%s\" matched successfully!\n", word);
            match = 1;
            break;
        }
    }
    return match;
}

/*
void *worker() {
    //lock mutex controlling client buffer
    pthread_mutex_lock(&client_buffer_mutex);

    //if buffer is empty
    while (client_buffer_size == 0) {
        //wait until client arrives
        pthread_cond_wait(&client_buffer_not_empty, &client_buffer_mutex);
    }

    //get socket from client buffer
    int socket = client_buffer[client_buffer_in];
    client_buffer_in = (client_buffer_in++) % CLIENT_BUFFER_SIZE;
    --client_buffer_size;

    //unlock mutex
    pthread_mutex_unlock(&client_buffer_mutex);

    //send signal that buffer is not full
    pthread_cond_signal(&client_buffer_not_full);

    //keep receiving words until the client disconnects
    //while client doesnt connect?
    while(1) {
        //bool to check if spelled correctly
        int iscorrect = 0;

        //allocate space to receive word
        char *word = calloc(DICT_BUF, 1);

        //receive word
        word = recv(socket, word, MAX_WORD_SIZE, 0);

        //search for word
        if (lookup(word) == 0) {
            printf("MISPELLED");
        }

        else {
            iscorrect = 1;
            printf("OK");
        }

        //print result to client


        //push the result to the log queue

    }
}
 */

void *logger(){
    //get the log queue

    //print results to log file
    //fprintf(LOG, "%s %s\n", word, iscorrect);
};




//socket test
/*

    int socketfd;
    socketfd = socket(AF_INET , SOCK_STREAM , 0);

    if (socketfd == -1) {
        perror("Could not create socket");
    }

    int listeningSocket = open_listenfd(somePort);
    int connectionSocket;

    connectionSocket = accept(listeningSocket, NULL, NULL);
    //connectionSocket now holds information about a connected client.


    close(socketfd);
    return 0;
}*/
