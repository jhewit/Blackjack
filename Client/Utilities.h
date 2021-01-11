/*
* Utilities.h
*
* CSS 432 Group Lucky No 13
*
* Constants and utility functions
*/

#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <pthread.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <sys/time.h>

/*
* Prompt player for action
*/
namespace Prompt {
	constexpr auto NAME = "Please enter a username (no spaces): ",
		GAME = "\n\nPlease enter a game ID from the follwing (-1 for new game)\n",
		LIST1 = "\nGame ID: ",
		LIST2 = "\nPlayers:",
		BET = "\n-------------------------------"
		"\n\tPlace your bet!\n------------------------------\n\nTable Rules: "
		"\n - Minimum Bet = $2\n - Maximum Bet = $10\n - Bet must be in $2 increments"
		"\n\n - (To Exit, Enter -1)\n$",
		TURN1 = "It's ",
		TURN2 = "'s turn!\n",
		TURN3 = "\n\nBelow are the options and their integer-only commands: "
		"\n - Play Commands:\t\t[1] = Hit	|	[3] = Double Down"
		"\n - Non-Play Commands:\t\t[0] = Stand	|	[2] = Surrender\n",
		NO_GAMES = "\nNo current games\n\n",
		JOINED = "\nGame joined, please wait for game to start\n",
		SKIP_ROUND = "\nOther players have quit, but new ones have joined, skipping to next round\n",
		GAME_OVER = "\nOther players have quit and game is now ending\n",
		DEALER = "---------------------------------------------"
		"\n\t\tIT's the Dealer's turn !\n----------------------------------------\n\n",
		ACTION = " chose to ",
		HIT = "hit!",
		SURRENDER = "surrender!",
		DOUBLE_DOWN = "double down!",
		STAND = "stand";
}

/*
* Responses that player input was bad
*/
namespace Bad {
	constexpr auto ARG = "usage: ./client serverAddress\n",
		NAME = "Sorry, this username is already taken, please try again: ",
		GAME = "Invalid game ID, please try again: ",
		BET = "Invalid bet amount, please try again: ",
		ACTION = "Invalid action, please try again: ",
		DOUBLE_DOWN = "\n!!Unable to Double Down -- Your balance is too low!!"
		"\nPlease either Hit, Surrender, or Stand.\n";
}

/*
* Byte sizes needed for variables
*/
namespace Size {
	constexpr auto USERNAME = 50,
		BUF = 2000;
}

/*
* Default values for variables and input
*/
namespace Default {
	constexpr auto EXPECTED_ARGS = 2,
		CREATE_ID = -1,
		DISPLAY_WAIT = 3,
		STARTER_WAIT = 5,
		MAX_GAMES = 5;
}

/*
* Bet values
*/
namespace Bet {
	constexpr auto QUIT = -1,
		MIN = 2,
		MAX = 10;
}

/*
* For logging system call failures
*/
namespace Failure {
	constexpr auto WRITE = "write() failure\n",
		CLOSE = "close() failure\n",
		SOCK = "socket() failure\n",
		SOCK_OPT = "setsockopt() failure\n",
		BIND = "bind() failure\n",
		LISTEN = "listen() failure\n",
		ACCEPT = "accept() failure\n",
		CONN = "connect() failure\n\n",
		CREATE = "pthread_create() failure\n",
		READ = "read() failure\n",
		FORK = "fork() failure\n",
		INIT = "pthread_mutex_init() failure\n",
		LOCK = "pthread_mutex_lock() failure\n",
		UNLOCK = "pthread_mutex_unlock() failure\n",
		TIME = "gettimeofday() failure\n";
};

/*
* For establishing TCP connections
*/
namespace Tcp {
	constexpr auto NO_FD = -1,
		MAX_CONNECTS = 10,
		ON = 1,
		PORT = 1243;
}

/*
* Possible player actions
*/
enum ACTION {
	STAND,
	HIT,
	SURRENDER,
	DOUBLE_DOWN
};

/*
* For checking status after bets
*/
enum ROUND_STATUS {
	CONTINUE,
	ADD_PLAYER,
	OVER
};

/*
* Variables for registering new player and adding to a game
*/
struct NewPlayer {
	explicit NewPlayer(int* fd) :sock{*fd} {}
	int sock, gameId;
	char buf[Size::BUF];
	std::string userName;
};

/*
* Map Game ID to driver
*/
using DriverMapping = std::pair<int, class Driver*>;

/*
* Map active player's username to socket file descriptor
*/
using PlayerMapping = std::pair<const std::string, int>;

/*
* Print the error message, perform garbage collection if necessary and then exit the process
*/
void gracefulExit(const char* msg, int fd = Tcp::NO_FD, bool perr = false);

/*
* Close the socket and optionally end thread
*/
void end(int sock, bool exitThread = true);

/*
* Wait for 5 seconds
*/
void wait(timeval* start, timeval* end, int time);
void gettime(timeval*);

/*
* Write to socket and check for errors
*/
void writeTo(int sock, const void* msg, int sz);
void writeTo(int sock, int msg);
void writeTo(int sock, bool msg);

/*
* Write to socket and check for errors
*/
int readFrom(int sock, void* buf, int sz);
int readFrom(int sock);
bool readBoolFrom(int sock);

/*
* Initialize, lock and unlock a mutex
*/
void init(pthread_mutex_t* mtx);
void lock(pthread_mutex_t* mtx);
void unlock(pthread_mutex_t* mtx);

/*
* Run a thread for the given function with the given arg
*/
void runThread(void* (*func) (void*), const int arg);
void runThread(void* (*func) (void*), const Driver* arg);
#endif
