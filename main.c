#include <string.h>
#include "network.h"

int main(int argc, char *argv[]) {
    //default dictionary and socket
    if (argc == 1) {
        if (!(DICTIONARY = fopen(DICT_DEFAULT, "r"))) {
            perror("Dictionary");
            exit(EXIT_FAILURE);
        }
        SOCKET = SOCKET_DEFAULT;
    }

    //passed a dictionary or socket, figure out which
    else if (argc == 2){
        char *endptr;

        //if no integers in argument, assume its a dictionary
        if (!(SOCKET = (int) strtol(argv[1], &endptr, 10))) {
            if (!(DICTIONARY = fopen(argv[1], "r"))) {
                perror("Unknown dictionary");
                exit(EXIT_FAILURE);
            }
            SOCKET = SOCKET_DEFAULT;
        }
    }

    //passed specific dictionary and socket
    else {
        if (!(DICTIONARY = fopen(argv[1], "r"))) {
            perror("Unknown dictionary");
            exit(EXIT_FAILURE);
        }
        char *endptr;

        if (!(SOCKET = (int) strtol(argv[2], &endptr, 10))) {
            perror("Issue reading socket");
            exit(EXIT_FAILURE);
        }
    }
    /*
    printf("Dictionary:\t%s\n", argv[1]);
    printf("Socket:\t%d\n", SOCKET);
     */

    //create log file
    FILE *log = fopen("log.txt", "w+");






    //load dictionary, queues
    //set up sync mechs for queues
        //main thread should wait for signal from worker if Q full
        //workers should wait for main if Q empty
        //semaphores
    //make worker threads
    //create socket on port
    //wait for connections
    //create client socket descriptor, add to queue
    //worker takes descriptor and waits for word
    //worker checks dictionary
    //worker writes to log
    //worker takes another word or terminates

    /* socket test */
    int socketfd;
    socketfd = socket(AF_INET , SOCK_STREAM , 0);

    if (socketfd == -1) {
        perror("Could not create socket");
    }




    /*	//Create an array of threads.
	pthread_t threadPool[WORKER_COUNT];
	int threadIDs[WORKER_COUNT];
	int i;
	printf("Launching threads.\n");
	for(i = 0; i < WORKER_COUNT; i++){
		threadIDs[i] = i;
		//Start running the threads.
		pthread_create(&threadPool[i], NULL, &threadFunction, &threadIDs[i]);
	}
	printf("All threads launched, waiting for them to quit.\n");
	for(i = 0; i < WORKER_COUNT; i++){
		//Wait for all threads to finish executing.
		pthread_join(threadPool[i], NULL);
	}
	printf("All threads completed.\n");
	return 0;
}
void* threadFunction(void* id){
	printf("Thread %d: Hello\n", *((int*) id));
	printf("Thread %d: Quitting\n", *((int*) id));




     int listeningSocket = open_listenfd(somePort);
int connectionSocket;

connectionSocket = accept(listeningSocket, NULL, NULL);
//connectionSocket now holds information about a connected client.

*/
    return 0;
}