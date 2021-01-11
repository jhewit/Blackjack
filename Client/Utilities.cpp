/*
* Utilities.cpp
*
* CSS 432 Group Lucky No 13
*
* Constants and utility functions
*/

#include "Utilities.h"

/*
* Print the error message, perform garbage collection if necessary and then exit the process
*/
void gracefulExit(const char* msg, int fd, bool perr) {

	if (perr) {
		perror(msg);
	}
	else {
		printf(msg);
	}
	if (fd != Tcp::NO_FD) {

		if (close(fd) < 0) {
			perror(Failure::CLOSE);
		}
	}

	exit(EXIT_FAILURE);
}

/*
* Close the socket and optionally end thread
*/
void end(int sock, bool exitThread) {

	if (close(sock) < EXIT_SUCCESS) {
		perror(Failure::CLOSE);
	}
	if (exitThread) {
		pthread_exit(nullptr);
	}
}

/*
* Wait for 5 seconds
*/
void wait(timeval* start, timeval* end, int time) {

	gettime(start);

	do {
		gettime(end);
	}
	while (end->tv_sec - start->tv_sec < time);
}
void gettime(timeval* timer) {
	
	if (gettimeofday(timer, nullptr) < 0) {
		perror(Failure::TIME);
	}
}

/*
* Write to socket and check for errors
*/
void writeTo(int sock, const void* msg, int sz) {

	if (write(sock, msg, sz) < 0) {
		perror(Failure::WRITE);
	}
}
void writeTo(int sock, int msg) {

	if (write(sock, &msg, sizeof(int)) < 0) {
		perror(Failure::WRITE);
	}
}
void writeTo(int sock, bool msg) {

	if (write(sock, &msg, sizeof(bool)) < 0) {
		perror(Failure::WRITE);
	}
}

/*
* Write to socket and check for errors
*/
int readFrom(int sock, void* buf, int sz) {

	int bytes = read(sock, buf, sz);
	if (bytes < 0) {
		perror(Failure::WRITE);
	}
	return bytes;
}
int readFrom(int sock) {

	int buf;
	if (read(sock, &buf, sizeof(int)) < 0) {
		perror(Failure::WRITE);
	}
	return buf;
}
bool readBoolFrom(int sock) {

	bool buf;
	if (read(sock, &buf, sizeof(bool)) < 0) {
		perror(Failure::WRITE);
	}

	return buf;
}

/*
* Initialize, lock and unlock a mutex
*/
void init(pthread_mutex_t* mtx) {

	if (pthread_mutex_init(mtx, nullptr) != 0) {
		perror(Failure::INIT);
	}
}
void lock(pthread_mutex_t* mtx) {

	if (pthread_mutex_lock(mtx) != 0) {
		perror(Failure::LOCK);
	}
}
void unlock(pthread_mutex_t* mtx) {

	if (pthread_mutex_unlock(mtx) != 0) {
		perror(Failure::UNLOCK);
	}
}

/*
* Run a thread for the given function with the given arg
*/
void runThread(void* (*func) (void*), const int arg) {

	pthread_t thread;
	if (pthread_create(&thread, nullptr, func, (void*) &arg) < 0) {
		perror(Failure::CREATE);
		///???
	}
}
void runThread(void* (*func) (void*), const Driver* arg) {

	pthread_t thread;
	if (pthread_create(&thread, nullptr, func, (void*) arg) < 0) {
		perror(Failure::CREATE);
		///???
	}
}
