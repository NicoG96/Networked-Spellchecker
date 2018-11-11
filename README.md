# Networked Spell-Checker
A multi-threaded spell-checker implemented over a local network
___
Program acts as a server for multiple clients, distributing worker threads to each for simultaneous execution. 

Clients that connect to the server have their socket addresses automatically added to a queue for worker threads to service

Synchronization dilemma handled by restricting buffer access with a series of condition variables and mutex locks

Program also contains an instance of a thread for record keeping. It monitors server activity and writes all results to an output log file 



## Usage

Launch the server and SSH into localhost. Then use `telnet localhost [PORT]` to access the port that the server is listening on. From there, the server will greet you with a welcome message and you are free to spell-check any word you desire. Words spelled correctly will print "OK" to your screen and "MISSPELLED" otherwise. Press escape key to exit the program.