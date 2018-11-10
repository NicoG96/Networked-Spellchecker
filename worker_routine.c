#include "network.h"

void *worker_routine(void* args) {
    server *serv = args;

    //lock clientQ
    pthread_mutex_lock(&serv->client_mutex);

    //if client Q is empty
    while (serv->client_count == 0) {
        pthread_cond_wait(&serv->client_not_empty, &serv->client_mutex);
    }

    //get socket from clientQ
    int socket = remove_client(serv);

    //send signal that Q is not full
    pthread_cond_signal(&serv->client_not_full);

    //unlock mutex
    pthread_mutex_unlock(&serv->client_mutex);

    char *msgError = "Problem receiving message";
    int bytesReturned;
    char *res;
    char *word;

    //keep receiving words until the client disconnects
    while(1) {
        //zero-initialize var to receive word
        word = calloc(DICT_BUF, 1);

        //receive word
        bytesReturned = (int) recv(socket, word, DICT_BUF, 0);
        //printf("%s", word);

        //if there was an error in the message reception
        if (bytesReturned < 0) {
            send(socket, msgError, strlen(msgError), 0);
            continue;
        }

        //or if the client pressed escape key
        if(word[0] == 27) {
            break;
        }

        //search for word, set the result equal to whether it was found or not
        int iscorrect = lookup(word);
        iscorrect ? res = "OK\n" : "MISSPELLED\n";

        //print results to client
        send(socket, res, strlen(res), 0);

        //push the result to the log queue, get lock first
        pthread_mutex_lock(&serv->log_mutex);

        //check if the buffer is full
        while(serv->log_count == BUFFER_MAX) {
            pthread_cond_wait(&serv->log_not_full, &serv->log_mutex);
        }

        //write to the logQ
        insert_log(serv, word, iscorrect);

        //signal that log Q isn't empty
        pthread_cond_signal(&serv->log_not_empty);

        //unlock the mutex
        pthread_mutex_unlock(&serv->log_mutex);
    }
    close(socket);
    return NULL;
}

int remove_client(server *serv) {
    int socket = *serv->client_buf;
    serv->client_buf = 0;

    //increment the index, and optionally loop back to 0 if we reach end of buffer
    serv->client_index = ++(serv->client_index) % BUFFER_MAX;

    //decrement the amount of clients in the buffer
    --(serv->client_count);

    return socket;
}

void insert_log(server *serv, char *word, int iscorrect) {
    char string[DICT_BUF];
    char *res;

    iscorrect ? res = "OK" : "MISSPELLED";

    //"[word]" is [OK/MISSPELLED]
    strcpy(string, "\"");
    strcat(string, word);
    strcat(string, "\" is ");
    strcat(string, res);

    //push string to the queue
    strcpy(*serv->log_buf, string);

    //increment the index, and optionally loop back to 0 if we reach end of buffer
    serv->log_index = ++(serv->log_index) % BUFFER_MAX;

    //increment the amount of logs in the buffer
    ++(serv->log_count);
}