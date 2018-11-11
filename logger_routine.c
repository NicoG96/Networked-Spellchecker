#include "network.h"

void *logger_routine(void *args){
    server *serv = args;

    while(1) {
        //lock the log queue
        pthread_mutex_lock(&serv->log_mutex);

        //check if it's empty
        while (serv->l_read_ptr == serv->l_write_ptr && serv->log_count == 0) {
            pthread_cond_wait(&serv->log_not_empty, &serv->log_mutex);
        }

        //get the log results
        char *result = remove_log(serv);

        //write results to log
        fprintf(LOG, "%s\n", result);

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