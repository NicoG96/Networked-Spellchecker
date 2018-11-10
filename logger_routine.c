#include "network.h"

void *logger_routine(void *args){
    server *serv = args;

    //create log file
    LOG = fopen("log.txt", "w+");

    while(1) {
        //lock the log queue
        pthread_mutex_lock(&serv->log_mutex);

        //check if it's empty
        while (&serv->log_count == 0) {
            pthread_cond_wait(&serv->log_not_empty, &serv->log_mutex);
        }



        //signal that Q isn't full
        pthread_cond_signal(&serv->log_not_full);

        //unlock the queue
        pthread_mutex_unlock(&serv->log_mutex);

        //print results to log file
        //fprintf(LOG, "%s %s\n", word, iscorrect);
    }
}