//---------------------------------Server.cpp-----------------------------------
//Description:
//------------------------------------------------------------------------------

#include "Driver.h"

#pragma region Globals

/*
* Mutexes for thread safety on shared resources
*/
pthread_mutex_t qMtx = PTHREAD_MUTEX_INITIALIZER,
				mapMtx = PTHREAD_MUTEX_INITIALIZER,
				idMtx = PTHREAD_MUTEX_INITIALIZER;

/*
* Queue of Driver's that are waiting for more than one player
* in order to start Game
*/
std::queue<DriverMapping> q;

/*
* Map Game ID to Driver
*/
std::map<int, Driver*> dMap;

/*
* Game ID generator (increment by 1)
* Current number of games
*/
int id{0}, games{0};

#pragma endregion

/*
* Locks and unlocks on mutexes for resources
* used by multiple threads
*/
#pragma region Mtx
void lockQ() {
	lock(&qMtx);
}
void lockMap() {
	lock(&mapMtx);
}
void lockId() {
	lock(&idMtx);
}
void unlockQ() {
	unlock(&qMtx);
}
void unlockMap() {
	unlock(&mapMtx);
}
void unlockId() {
	unlock(&idMtx);
}
#pragma endregion

/*
* Add data to / remove data from resources used by multiple threads,
* requires locking and unlocking mutexes
*/
#pragma region AddTo / RemoveFrom
void addToQ(const DriverMapping* mapping) {
	lockQ();
	q.push(*mapping);
	unlockQ();
}
void addToMap(const DriverMapping* mapping) {
	lockMap();
	dMap.emplace(*mapping);
	unlockMap();
}
void removeFromMap(Driver* driver) {
	lockMap();
	dMap.erase(driver->gameId());
	unlockMap();
	delete driver;
}
#pragma endregion


#pragma region Game Starter

/*
* Start the Game by letting Driver run the rounds
*/
void* startGame(void* arg) {
	
	Driver* driver = (Driver*) arg;
	do {
		driver->startRound();
	}
	while (!driver->gameOver());
	removeFromMap(driver);
	pthread_exit(nullptr);
}

/*
* Return true if q is emtpy
*/
bool qEmpty() {
	
	lockQ();
	bool empty = q.empty();
	unlockQ();
	return empty;
}

/*
* Run a thread to start the driver's new game
*/
void runNewGame(const Driver* driver) {
	runThread(startGame, driver);
}

/*
* Start next reeady Game in readyQ
*/
void checkIfReady(const DriverMapping* mapping) {

	if (mapping->second->gameReady()) {
		runNewGame(mapping->second);
	}
	else {
		addToQ(mapping);
	}
}

/*
* Thread to continuously start a new thread for each Game
* that is ready to begin
*/
void* gameStarter(void*) {

	timeval start, end;

	while (true) {
		while (!qEmpty()) {
			lockQ();
			DriverMapping mapping = q.front();
			q.pop();
			unlockQ();
			checkIfReady(&mapping);
		}
		wait(&start, &end, Default::STARTER_WAIT);
	}
	pthread_exit(nullptr);
}

/*
* Start thread to continuously check if waiting
* Drivers' Games are ready to start and run in own thread
*/
void runGameStarter() {
	runThread(gameStarter, 0);
}

#pragma endregion

#pragma region TCP

/*
* Initialize the socket data struct
*/
void initialize(sockaddr_in* addr) {

	bzero((char*)addr, sizeof(*addr));
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = htonl(INADDR_ANY);
	addr->sin_port = htons(Tcp::PORT);
}

/*
* Create a tcp socket and return file descriptor
*/
int tcpSocket() {

	int fd = socket(AF_INET, SOCK_STREAM, 0);

	if (fd < EXIT_SUCCESS) {
		gracefulExit(Failure::SOCK, Tcp::NO_FD, true);
	}

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*) &Tcp::ON, sizeof(int)) < EXIT_SUCCESS) {
		gracefulExit(Failure::SOCK_OPT, fd, true);
	}

	return fd;
}

/*
* Bind the socket address with the file descriptor and
* set it to listen for a maximum number of connection requests
*/
void setRequestSocket(int fd, sockaddr_in* addr) {

	if (bind(fd, (sockaddr*)addr, sizeof(*addr)) < EXIT_SUCCESS) {
		gracefulExit(Failure::BIND, fd, true);
	}

	if (listen(fd, Tcp::MAX_CONNECTS) < EXIT_SUCCESS) {
		gracefulExit(Failure::LISTEN, fd, true);
	}
}

/*
* Establish socket to accept TCP connection requests
*/
int tcpConnectRequestSock() {

	sockaddr_in addr;
	initialize(&addr);

	int sockfd = tcpSocket();
	setRequestSocket(sockfd, &addr);

	return sockfd;
}

#pragma endregion

#pragma region Main Menu

#pragma region UserName

/*
* Read user name from socket
*/
void readUserName(NewPlayer* player) {
	readFrom(player->sock, player->buf, Size::USERNAME);
	player->userName = std::string(player->buf);
}

/*
* Return true if username taken
*/
bool userNameTaken(const std::string* username) {

	lockMap();
	std::map<int, Driver*> copy{dMap};
	unlockMap();

	for (const auto mapping : copy) {
		if (mapping.second->hasUserName(username)) {
			return true;
		}
	}
	return false;
}

/*
* Return true if username is valid (not taken)
*/
bool invalidUserName(const NewPlayer* player) {

	bool invalid = userNameTaken(&player->userName);
	writeTo(player->sock, invalid);
	return invalid;
}

/*
* Register new player with a username
*/
void registerPlayer(NewPlayer* player) {

	do {
		readUserName(player);
	}
	while (invalidUserName(player));
}

#pragma endregion

#pragma region Join/Create Game

/*
* Return true if Driver's game can be joined
*/
bool joinable(const Driver* driver) {
	return !driver->gameOver() && !driver->gameFull();
}

/*
* Create list of available games and sent to new player
*/
std::string getList() {

	stringstream ss;

	lockMap();
	std::map<int, Driver*> copy{dMap};
	unlockMap();

	for (const auto mapping : copy) {
		if (joinable(mapping.second)) {
			ss << mapping.second->userNameList();
		}
	}
	return ss.str();
}

/*
* Return true if no games to display to new player
*/
bool noGames(const std::string* list) {
	return list->empty();
}

/*
* Add a message saying there are no games
*/
void addNoGames(std::string* list) {
	list->append(Prompt::NO_GAMES);
}

/*
* List available games for new player to join
*/
void listGames(int sockfd) {

	std::string list{getList()};
	if (noGames(&list)) {
		addNoGames(&list);
	}
	writeTo(sockfd, list.c_str(), list.size());
}

/*
* Read game ID sent by client in socket,
* store in new players gameId
*/
void readGameId(NewPlayer* player) {
	player->gameId = readFrom(player->sock);
}

/*
* Game ID generator (increment by 1)
*/
int nextId() {
	int next;
	lockId();
	next = id++;
	unlockId();
	return next;
}

/*
* Create new game with new player
*/
void newGame(NewPlayer* player) {

	DriverMapping mapping{player->gameId = nextId(), new Driver{player}};
	addToQ(&mapping);
	addToMap(&mapping);
}

/*
* Return the Driver with the given id
*/
Driver* getDriver(int id) {
	lockMap();
	Driver* driver = dMap.count(id) ? dMap.at(id) : nullptr;
	unlockMap();
	return driver;
}

/*
* Return true if the new player can join Game in Driver
*/
bool canJoin(const NewPlayer* player) {

	Driver* driver = getDriver(player->gameId);
	return driver && !driver->gameOver() && driver->addPlayer(player);
}

/*
* Add new player to an existing game
*/
bool joinGame(NewPlayer* player) {

	bool joined{true};

	if (player->gameId == Default::CREATE_ID) {
		newGame(player);
	}
	else {
		joined = canJoin(player);
	}

	writeTo(player->sock, joined);
	return joined;
}

/*
* Return the game ID chosen by client
*/
void chooseGame(NewPlayer* player) {

	listGames(player->sock);

	do {
		readGameId(player);
	}
	while (!joinGame(player));
}	

#pragma endregion

/*
* Register new player with a unique username,
* display all available games,
* add new player to existing game or
* create a new game for new player
*/
void* newPlayer(void* arg) {

	NewPlayer player{(int*) arg};
	registerPlayer(&player);
	chooseGame(&player);
	pthread_exit(nullptr);
}

/*
* Run a thread for a new player wanting to connect
*/
void runNewPlayer(int newFd) {
	runThread(newPlayer, newFd);
}

#pragma endregion

/*
* Create a TCP connection request socket,
* create a thread for each connection request,
* register the new player and new socket in the thread
*/
int main() {

	runGameStarter();

	int requestFd = tcpConnectRequestSock(), newFd;
	sockaddr_in newAddr;
	socklen_t newAddrSz = sizeof(newAddr);

	while (true) {

		newFd = accept(requestFd, (sockaddr*) &newAddr, &newAddrSz);
		if (newFd < EXIT_SUCCESS) {
			perror(Failure::ACCEPT);
			continue;
		}
		runNewPlayer(newFd);
	}
}
