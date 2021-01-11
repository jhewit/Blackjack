/*
* Client.cpp
*
* CSS 432 Group Lucky No 13
*/

#include <iostream>
#include <arpa/inet.h>
#include <climits>
#include "Utilities.h"

#pragma region Globals

/*
* Socket file descriptor connected to server
*/
int sock{0};

/*
* Buffer to read data sent in socket by server
*/
char inputBuf[Size::BUF];
int bytes{0};

/*
* Buffer to write data in socket to server
*/
std::string outputBuf;

/*
* Buffer to write int data
*/
int intBuf;

/*
* Player's username
*/
char username[Size::USERNAME];

#pragma endregion

#pragma region TCP

/*
* Initialize socket data structs
*/
void initialize(sockaddr_in* addr, const char* hostName) {

	bzero((char*)addr, sizeof(addr));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(Tcp::PORT);
	struct hostent* host = gethostbyname(hostName);
	addr->sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*) * host->h_addr_list));
}

/*
* Create TCP socket and return file descriptor
*/
void tcpSocket() {

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < EXIT_SUCCESS) {
		gracefulExit(Failure::SOCK, Tcp::NO_FD, true);
	}
}

/*
* Request TCP connection with server
*/
void connect(sockaddr_in* addr) {

	if (connect(sock, (sockaddr*)addr, sizeof(*addr)) < EXIT_SUCCESS) {
		gracefulExit(Failure::CONN, sock, true);
	}
}

/*
* Establish TCP connection with server and return socket file descriptor
*/
void tcpConnection(const char* hostName) {

	sockaddr_in addr;
	initialize(&addr, hostName);
	tcpSocket();
	connect(&addr);
}

#pragma endregion

#pragma region Socket read/write

/*
* Return true if input was not a valid int
*/
bool inputNotInt() {
	bool notInt = !std::cin.good();
	if (notInt) {
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
	}
	return notInt;
}

/*
* Get user input as int
*/
void getInput() {
	do {
		std::cin >> intBuf;
	}
	while (inputNotInt());
}

/*
* Read data from socket and display to console
*/
void readFromServer() {
	bytes = readFrom(sock, inputBuf, Size::BUF);
	printf("%.*s", bytes, inputBuf);
}

/*
* Get user input from console and send throuh socket
*/
void writeToServer() {
	std::cin >> outputBuf;
	writeTo(sock, outputBuf.c_str(), outputBuf.size());
}
void writeIntToServer() {
	getInput();
	writeTo(sock, intBuf);
}

#pragma endregion

#pragma region Register Player

/*
* Prompt new player to choose a user name
*/
void promptUserName() {
	printf(Prompt::NAME);
}

/*
* Return true if server's response is bad username
*/
bool userNameTaken() {
	bool taken = readBoolFrom(sock);
	if (taken) {
		printf(Bad::NAME);
	}
	return taken;
}

/*
* Save username from buffer
*/
void saveUserName() {
	strncpy(username, outputBuf.c_str(), outputBuf.size());
}

/*
* Register the new player:
*	- read user name prompt froms server
*	- write user name choice to server
*	- read server's response
*	- retry user name selection while user name taken
*	-
*/
void registerPlayer() {

	promptUserName();
	do {
		writeToServer();
	}
	while (userNameTaken());
	saveUserName();
}

#pragma endregion

#pragma region Join/Create Game

/*
* Prompt player to select a game
*/
void promptGame() {
	printf(Prompt::GAME);
}

/*
* Return true if string says game was joined
*/
bool gameJoined() {
	bool joined = readBoolFrom(sock);
	if (!joined) {
		printf(Bad::GAME);
	}
	return joined;
}

/*
* Print to player that game is joined and need to wait
*/
void printJoined() {
	printf(Prompt::JOINED);
}

/*
* Read the list of games from the server and display to player,
* send player's chosen game ID to server,
* make player re-choose while chosen game is full or ending
*/
void chooseGame() {

	promptGame();
	readFromServer();
	do {
		writeIntToServer();
	}
	while (!gameJoined());
	printJoined();
}

#pragma endregion

#pragma region Player Turn (Gameplay)

/*
* Wait for gameplay until server signals start
*/
void waitForStart() {
	readBoolFrom(sock);
}

/*
* Return true if bet amount is valid
*/
bool validBet() {

	bool valid = (intBuf == Bet::QUIT) ||
		(Bet::MIN <= intBuf && intBuf <= Bet::MAX && intBuf % 2 == 0);

	if (!valid) {
		printf(Bad::BET);
	}
	return valid;
}

/*
* Place player's bet
*/
void placeBet() {

	printf(Prompt::BET);

	do {
		getInput();
	}
	while (!validBet());

	writeTo(sock, intBuf);
	if (intBuf == Bet::QUIT) {
		exit(0);

	}
}

/*
* Check round status after betting
*/
ROUND_STATUS checkStatus() {

	return (ROUND_STATUS) readFrom(sock);
}

/*
* Display that round was skipped because too many players left,
* but others joined so next round can start
*/
void displayRoundSkipped() {
	printf(Prompt::SKIP_ROUND);
	sleep(Default::DISPLAY_WAIT);
}

/*
* Display all player's cards
*/
void displayCards() {
	readFromServer();
}

/*
* Display who's turn it is
*/
void displayTurn() {
	printf(Prompt::TURN1);
	readFromServer();
	printf(Prompt::TURN2);	
}

/*
* Return signal from server
*/
bool nextTurn() {
	return readBoolFrom(sock);
}

/*
* Return true if player's turn
*/
bool myTurn() {
	return strncmp(username, inputBuf, bytes) == 0;
}

/*
* Return true is user's input is a valid action
*/
bool validAction() {
	bool valid = STAND <= intBuf && intBuf <= DOUBLE_DOWN;
	if (!valid) {
		printf(Bad::ACTION);
	}
	return valid;
}

/*
* Start the player's turn
*/
void startTurn() {
	printf(Prompt::TURN3);
	do {
		getInput();
	}
	while (!validAction());
	writeTo(sock, intBuf);
}

/*
* Go through all players turns
*/
void takeTurns() {

	while (nextTurn()) {
		displayTurn();
		if (myTurn()) {
			startTurn();
		}
		displayCards();
		sleep(Default::DISPLAY_WAIT);
	}
}

/*
* Display game stats
*/
void displayStats() {
	readFromServer();
	sleep(Default::DISPLAY_WAIT);
}

/*
* Server sends true if next round
*/
bool nextRound() {
	return readBoolFrom(sock);
}

/*
* Print to client console that game is over
*/
void gameOver() {
	printf(Prompt::GAME_OVER);
	exit(0);
}

/*
* Play rounds of game until over
*/
void play() {

	do {
		waitForStart();
		placeBet();

		switch (checkStatus()) {

		case CONTINUE:
			displayCards();
			takeTurns();
			displayCards();
			sleep(Default::DISPLAY_WAIT);
			displayStats();
			break;

		case ADD_PLAYER:
			displayRoundSkipped();
			break;

		case OVER:
			gameOver();
		}
	}
	while (nextRound());

	gameOver();
}

#pragma endregion

/*
* Make sure the right amount of arguments passed in
*/
void checkArgCount(int argc) {

	if (argc != Default::EXPECTED_ARGS) {
		gracefulExit(Bad::ARG);
	}
}

/*
* Check that server's address was given,
* create a tcp connection with server,
* register with a username,
* choose a game to join or create one,
* play the game
*/
int main(int argc, char* argv[]) {

	checkArgCount(argc);
	tcpConnection(argv[1]);
	registerPlayer();
	chooseGame();
	play();
}
