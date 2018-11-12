#include "network.h"

void *logger_routine(void *args){
    server *serv = args;

    /* TEST */
    /*
    printf("CLIENT COUNT: \t%d\n", serv->client_count);
    printf("LOG COUNT: \t%d\n", serv->log_count);
    printf("CLIENT READ PTR: \t%d\n", serv->c_read_ptr);
    printf("CLIENT WRITE PTR: \t%d\n", serv->c_write_ptr);
    printf("LOG READ PTR: \t%d\n", serv->l_read_ptr);
    printf("LOG WRITE PTR: \t%d\n", serv->l_write_ptr);
     */

    while(1) {
        //lock the log queue
        pthread_mutex_lock(&serv->log_mutex);

        //check if it's empty
        while (serv->l_read_ptr == serv->l_write_ptr && serv->log_count == 0) {
            pthread_cond_wait(&serv->log_not_empty, &serv->log_mutex);
        }

        /* TEST */
        /*
        for(int i = 0; i < BUFFER_MAX; i++) {
            printf("pre-log-removal[%d]:\t%d\n", i, logs[i]);
        }
        printf("LOG WRITE PTR: \t%d\n", serv->l_write_ptr);
        printf("LOG COUNT: \t%d\n", serv->log_count);
        */

        //get the log results
        char *result = remove_log(serv);

        /* TEST */
        /*
        for(int i = 0; i < BUFFER_MAX; i++) {
            printf("post-log-removal[%d]:\t%d\n", i, logs[i]);
        }
        printf("LOG WRITE PTR: \t%d\n", serv->l_write_ptr);
        printf("LOG COUNT: \t%d\n", serv->log_count);
        */

        //write results to log
        fprintf(LOG, "%s\n", result);

        //force write of IO buffer
        fflush(LOG);

        //signal that Q isn't full
        pthread_cond_signal(&serv->log_not_full);

        //unlock the queue
        pthread_mutex_unlock(&serv->log_mutex);
    }
}

char *remove_log(server *serv) {
    char *result = logs[serv->l_read_ptr];

    //clear index
    logs[serv->l_read_ptr] = (char *) calloc(1, sizeof(char *));

    //increment the read pointer
    serv->l_read_ptr = (++serv->l_read_ptr) % BUFFER_MAX;

    //decrement the total log count
    --serv->log_count;

    return result;
}