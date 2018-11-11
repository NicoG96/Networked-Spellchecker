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

    ////////////


    //create struct that holds all condition vars and mutex locks
    server *serv = malloc(sizeof(*serv));

    //initialize the server
    server_init(serv);

    /* TEST */
    /*
    printf("CLIENT COUNT: \t%d\n", serv->client_count);
    printf("LOG COUNT: \t%d\n", serv->log_count);
    printf("CLIENT READ PTR: \t%d\n", serv->c_read_ptr);
    printf("CLIENT WRITE PTR: \t%d\n", serv->c_write_ptr);
    printf("LOG READ PTR: \t%d\n", serv->l_read_ptr);
    printf("LOG WRITE PTR: \t%d\n", serv->l_write_ptr);
     */

    //create thread pool
    pthread_t workers[BUFFER_MAX];

    //add threads to the pool
    for (int i = 0; i < BUFFER_MAX; i++) {
        pthread_create(&workers[i], NULL, worker_routine, (void *)serv);
    }

    //open log file
    if ((LOG = fopen("log.txt", "w+")) == NULL) {
        perror("Log file creation");
        exit(EXIT_FAILURE);
    }

    //create thread for logger
    pthread_t logger;
    pthread_create(&logger, NULL, logger_routine, (void *) serv);

    //create socket variable for client connections
    int connected_socket;

    char *greeting = "Hello! Type a word to spell-check it.\n";

    //start accepting clients
    while (1) {
        if ((connected_socket = accept(listen_socket, NULL, NULL)) < 1) {
            perror("Can't connect to client");
            break;
        }
        puts("Client connected.");

        //send greeting message to client with instructions
        send(connected_socket, greeting, strlen(greeting), 0);

        //get clientsQ lock
        pthread_mutex_lock(&serv->client_mutex);

        //check if clientsQ is full. Using circular array , so full when read == write && maxed out elements
        while (serv->c_read_ptr == serv->c_write_ptr && serv->client_count == BUFFER_MAX) {
            pthread_cond_wait(&serv->client_not_full, &serv->client_mutex);
        }

        //add client socket to Q
        insert_client(serv, connected_socket);

        /* TEST */
        /*
        for(int i = 0; i < BUFFER_MAX; i++) {
            printf("%d\n", clients[i]);
        }
        printf("CLIENT WRITE PTR: \t%d\n", serv->c_write_ptr);
        printf("CLIENT COUNT: \t%d\n", serv->client_count);
        */

        //signal workers
        pthread_cond_signal(&serv->client_not_empty);

        //unlock Q
        pthread_mutex_unlock(&serv->client_mutex);
    }

    //clean up
    close(listen_socket);
    close(connected_socket);
    fclose(LOG);
    return 0;
}

void server_init(server *serv) {
    serv->client_count = 0;
    serv->log_count = 0;

    serv->c_read_ptr = 0;
    serv->c_write_ptr = 0;
    serv->l_read_ptr = 0;
    serv->l_write_ptr = 0;

    if (pthread_mutex_init(&serv->client_mutex, NULL) != 0) {
        perror("Client mutex init");
    }

    if (pthread_mutex_init(&serv->log_mutex, NULL) != 0) {
        perror("Log mutex init");
    }

    if (pthread_cond_init(&serv->client_not_empty, NULL) != 0) {
        perror("Client not empty init");
    }

    if (pthread_cond_init(&serv->log_not_empty, NULL) != 0) {
        perror("Log not empty init");
    }

    if (pthread_cond_init(&serv->client_not_full, NULL) != 0) {
        perror("Client not full init");
    }

    if (pthread_cond_init(&serv->log_not_full, NULL) != 0) {
        perror("Log not full init");
    }

    for(int i = 0; i < BUFFER_MAX; i++) {
        clients[i] = (int) calloc(1, sizeof(int));
        logs[i] = (char *) calloc(1, sizeof(char *));
    }
}

void insert_client(server *serv, int socket) {
    //printf("%d\n", socket);

    //insert into client buffer
    clients[serv->c_write_ptr] = socket;

    //increment the write pointer
    serv->c_write_ptr = (++serv->c_write_ptr) % BUFFER_MAX;

    //increment the total count
    ++serv->client_count;
}