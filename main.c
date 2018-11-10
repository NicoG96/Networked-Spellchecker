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

        //if the first argument isn't an int
        if (!(LISTEN_PORT = (int) strtol(argv[1], &endptr, 10))) {
            //assume it's a dictionary
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
        //if the port isn't a legal port
        if (LISTEN_PORT < 1024 || LISTEN_PORT > 65535) {
            perror("Port out of range (Must be between 1025 & 65534)");
            exit(EXIT_FAILURE);
        }
    }
    /*
    printf("Dictionary:\t%s\n", argv[1]);
    printf("Socket:\t%d\n", SOCKET);
     */

    //create a listening socket on the specified port
    int listen_socket;
    if ((listen_socket = open_listenfd(LISTEN_PORT)) < 0) {
        perror("Couldn't open listening socket");
        exit(EXIT_FAILURE);
    }

    //create struct that holds all condition vars and mutex locks
    server *serv = malloc(sizeof(*serv));

    //initialize the server
    server_init(serv);

    //create thread pool
    pthread_t workers[BUFFER_MAX];

    //add threads to the pool
    /*
    for (int i = 0; i < BUFFER_MAX; i++) {
        pthread_create(&workers[i], NULL, worker_routine(serv), 0);
    }
    pthread_create(&workers[0], NULL, worker_routine(serv), 0);
     */

    //create thread for logger
    /*
    pthread_t logger;
    pthread_create(&logger, NULL, logger_routine(serv), 0);
     */

    //create socket variable for client connections
    int connected_socket;

    /////////////////////////


    /*
    connected_socket = accept(listen_socket, NULL, NULL);
    char* clientMessage = "Hello!\n";
    send(connected_socket, clientMessage, strlen(clientMessage), 0);
    int bytesReturned;
    char recvBuffer[DICT_BUF];
    recvBuffer[0] = '\0';
    char* msgPrompt = ">>>";
    char* msgError = "I didn't get your message. ):\n";
    char* msgClose = "Goodbye!\n";
    char *msgResponse = "Got ur word chief.\n";




    //start accepting clients
    while (1) {
        //allocate space to receive word
        char *word = calloc(DICT_BUF, 1);
        memset(recvBuffer, '\0', DICT_BUF * sizeof(char));

        send(connected_socket, msgPrompt, strlen(msgPrompt), 0);
        bytesReturned = (int) recv(connected_socket, recvBuffer, DICT_BUF, 0);


        if(bytesReturned == -1){
            send(connected_socket, msgError, strlen(msgError), 0);
        }

        else if(recvBuffer[0] == 27){
            send(connected_socket, msgClose, strlen(msgClose), 0);
            break;
        }

        else {
            if (lookup(recvBuffer) > 0) {
                send(connected_socket, "OK", strlen("OK"), 0);
            }

            else {
                send(connected_socket, "MISSPELLED", strlen("MISSPELLED"), 0);
            }
        }




        //get clientsQ lock
        pthread_mutex_lock(&clientQ_mutex);

        //check if clientsQ is full
        while(count == CLIENT_BUF) {
            pthread_cond_wait(&clientQ_not_full, &clientQ_mutex);
        }

        //add client socket to Q
        clients[prod_ptr] = connected_socket;

        prod_ptr = (prod_ptr++) % CLIENT_BUF;
        ++count;

        //signal workers
        pthread_cond_signal(&clientQ_not_empty);

        //unlock Q
        pthread_mutex_unlock(&clientQ_mutex);

        break;

    }
     */


    close(listen_socket);
    //close(connected_socket);
    return 0;
}

void server_init(server *serv) {
    serv->client_count = 0;
    serv->log_count = 0;

    serv->client_index = 0;
    serv->log_index = 0;

    pthread_mutex_init(&serv->client_mutex, NULL);
    pthread_mutex_init(&serv->log_mutex, NULL);

    pthread_cond_init(&serv->client_not_empty, NULL);
    pthread_cond_init(&serv->log_not_empty, NULL);

    pthread_cond_init(&serv->client_not_full, NULL);
    pthread_cond_init(&serv->log_not_full, NULL);

    //init client and log buffers
}