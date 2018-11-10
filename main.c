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

    //start accepting clients
    while (1) {
        connected_socket = accept(listen_socket, NULL, NULL);

        //exit if client can't be connected
        if (connected_socket < 1) break;

        //get clientsQ lock
        pthread_mutex_lock(&serv->client_mutex);

        //check if clientsQ is full. Using circular array , so full when read == write && maxed out elements
        while (serv->c_read_ptr == serv->c_write_ptr && serv->client_count == BUFFER_MAX) {
            pthread_cond_wait(&serv->client_not_full, &serv->client_mutex);
        }

        //add client socket to Q
        insert_client(serv, connected_socket);

        //signal workers
        pthread_cond_signal(&serv->client_not_empty);

        //unlock Q
        pthread_mutex_unlock(&serv->client_mutex);
    }

    close(listen_socket);
    close(connected_socket);
    return 0;
}

void server_init(server *serv) {
    serv->client_count = 0;
    serv->log_count = 0;

    serv->c_read_ptr = -1;
    serv->c_write_ptr = -1;
    serv->l_read_ptr = -1;
    serv->l_write_ptr = -1;

    pthread_mutex_init(&serv->client_mutex, NULL);
    pthread_mutex_init(&serv->log_mutex, NULL);

    pthread_cond_init(&serv->client_not_empty, NULL);
    pthread_cond_init(&serv->log_not_empty, NULL);

    pthread_cond_init(&serv->client_not_full, NULL);
    pthread_cond_init(&serv->log_not_full, NULL);
}

void insert_client(server *serv, int socket) {
    //insert into client buffer
    clients[serv->c_write_ptr] = socket;

    //increment the write pointer
    serv->c_write_ptr = (++serv->c_write_ptr) % BUFFER_MAX;

    //increment the total count
    ++serv->client_count;
}