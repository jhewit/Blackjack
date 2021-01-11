/*
* Driver.cpp
*
* CSS 432 Group Lucky No 13
*/

#include "Driver.h"

/*
* Functions ran to simulate the sequential actions
* of each round
*/
#pragma region Round

/*
* Add player to active state
*/
void Driver::addActivePlayer(const PlayerMapping* mapping) {
	addToMap(mapping);
	game->addPlayer(mapping->first);
}

/*
* Add all waiting players to active map
*/
void Driver::addWaitingPlayers() {

	while (!qEmpty()) {
		lockQ();
		PlayerMapping mapping = q.front();
		q.pop();
		unlockQ();
		addActivePlayer(&mapping);
	}
}

/*
* Send signal to all players
*/
void Driver::signal(bool sig) const {
	for (const auto mapping : map) {
		writeTo(mapping.second, sig);
	}
}

/*
* Check if players quitted at betting
*/
void Driver::checkQuits() {

	while (!quitQ.empty()) {
		PlayerMapping mapping = quitQ.front();
		quitQ.pop();
		removePlayer(&mapping);
	}
}

/*
* Have each player place their bet
*/
void Driver::placeBets() {

	int bet;
	for (const auto mapping: map) {
		if ((bet = readFrom(mapping.second)) == Bet::QUIT) {
			quitQ.push(mapping);
		}
		else {
			game->placeBet(mapping.first, bet);
		}
	}
}

/*
* Check status for round after betting
*/
ROUND_STATUS Driver::checkStatus() const {

	ROUND_STATUS status{getStatus()};
	display(status);
	return status;
}
ROUND_STATUS Driver::getStatus() const {

	int names{namesSz()}, active{map.size()};

	//if (active > 1) {
	//	printf("CONTINUE\n");
	//	return CONTINUE;
	//}

	//if (names > 1) {
	//	printf("ADD_PLAYER\n");
	//	return ADD_PLAYER;
	//}

	//printf("OVER\n");
	//return OVER;

	return (active > 1) ? CONTINUE : ((names > 1) ? ADD_PLAYER : OVER);
}

/*
* Read the players action from socket
*/
ACTION Driver::readAction(int sock) {
	return (ACTION) readFrom(sock);
}

/*
* Player quits game
*/
void Driver::quit(const PlayerMapping* mapping) {
	game->removePlayer(mapping->first);
	removePlayer(mapping);
}

/*
* Switch state for given action
*/
bool Driver::switchAction(stringstream* ss, const std::string* username, ACTION action) {

	switch (action) {
	case HIT:
		*ss << Prompt::HIT;
		return game->hit(*username);
	case SURRENDER:
		*ss << Prompt::SURRENDER;
		return game->surrender(*username);
	case DOUBLE_DOWN:
		*ss << Prompt::DOUBLE_DOWN;
		return game->doubleDown(*username);
	case STAND:
		*ss << Prompt::STAND;
	default:
		return false;
	}
}

/*
* Do the player's given action
*/
bool Driver::doAction(const PlayerMapping* mapping, ACTION action) {
	
	stringstream ss;
	ss << '\n' << mapping->first << Prompt::ACTION;

	bool canHit = switchAction(&ss, &mapping->first, action);

	ss << '\n' << game->displayCards();
	display(ss.str().c_str());

	return canHit;
}

/*
* Signal next turn is starting
*/
void Driver::signalTurn() const {
	signal(true);
}

/*
* Signal player's turns are over
*/
void Driver::signalTurnsOver() const {
	signal(false);
}

/*
* Given player takes their turn
*/
void Driver::takeTurn(PlayerMapping* mapping) {

	ACTION action;

	do {
		signalTurn();
		displayTurn(&mapping->first);
		action = readAction(mapping->second);
	}
	while (doAction(mapping, action));
}

/*
* Have each player take their turn
*/
void Driver::takeTurns() {

	for (auto mapping: map) {
		takeTurn(&mapping);
	}
	signalTurnsOver();
}

/*
* Dealer actions
*/
void Driver::dealer() {
	game->dealerActions();
	displayCards();
	game->settleBets();
	wait(&start, &end, Default::DISPLAY_WAIT * 2);
}

/*
* Check if another round to go
*/
void Driver::checkIfNextRound() {
	bool nextRound = namesSz() > 1;
	setOver(!nextRound);
	signal(nextRound);
}

/*
* Begin a round in the Game
*/
void Driver::startRound() {

	addWaitingPlayers();
	signal(true);
	placeBets();
	checkQuits();

	switch (checkStatus()) {

	case CONTINUE:
		game->startRound();
		signal(true);
		displayCards();
		takeTurns();
		dealer();
		displayStats();
		checkIfNextRound();
		break;

	case ADD_PLAYER:
		signal(true);
		break;

	case OVER:
		setOver(true);
	}

}

#pragma endregion

/*
* Initialization, locks and unlocks on mutexes for resources
* used by multiple threads
*/
#pragma region Mtx
void Driver::initAllMtx() const {
	init(&namesMtx);
	init(&qMtx);
	init(&readyMtx);
}
void Driver::lockNames() const {
	lock(&namesMtx);
}
void Driver::lockQ() const {
	lock(&qMtx);
}
void Driver::lockReady() const {
	lock(&readyMtx);
}
void Driver::unlockNames() const {
	unlock(&namesMtx);
}
void Driver::unlockQ() const {
	unlock(&qMtx);
}
void Driver::unlockReady() const {
	unlock(&readyMtx);
}
#pragma endregion

/*
* Add to / remove from resources used by multiple threads,
* requires locking and unlocking mutexes
*/
#pragma region Names, Q, Map
void Driver::addToNames(const std::string* username) {
	lockNames();
	names.insert(*username);
	unlockNames();
}
void Driver::addToQ(const std::string* username, int sock) {
	lockQ();
	q.push({ *username, sock });
	unlockQ();
}
void Driver::addToMap(const std::string* username, int sock) {
	map.emplace(*username, sock);
}
void Driver::addToMap(const PlayerMapping* mapping) {
	map.emplace(*mapping);
}
void Driver::removeFromNames(const std::string* username) {
	lockNames();
	names.erase(*username);
	unlockNames();
}
void Driver::removeFromMap(const std::string* username) {
	map.erase(*username);
}
bool Driver::qEmpty() const {
	lockQ();
	bool empty = q.empty();
	unlockQ();
	return empty;
}
int Driver::namesSz() const {
	lockNames();
	int sz = names.size();
	unlockNames();
	return sz;
}
#pragma endregion

/*
* Adding or removing player,
* checking for their username,
* printing all usernames
*/
#pragma region Player
bool Driver::hasUserName(const std::string* userName) const {
	lockNames();
	bool has = names.count(*userName);
	unlockNames();
	return has;
}
bool Driver::addPlayer(const NewPlayer* player) {

	if (!gameFull()) {
		addToNames(&player->userName);
		addToQ(&player->userName, player->sock);
		setReady();
		return true;
	}
	return false;
}
void Driver::removePlayer(const PlayerMapping* mapping) {
	close(mapping->second);
	removeFromNames(&mapping->first);
	removeFromMap(&mapping->first);
	game->removePlayer(mapping->first);
}
std::string Driver::userNameList() const {

	std::stringstream ss;
	ss << Prompt::LIST1 << id << Prompt::LIST2;

	lockNames();
	for (const auto name : names) {
		ss << ' ' << name;
	}
	unlockNames();

	ss << "\n\n";
	return ss.str();
}
#pragma endregion

/*
* Checking Game's ID, ready or over status
*/
#pragma region Game
int Driver::gameId() const {
	return id;
}
bool Driver::gameReady() const {
	lockReady();
	bool isReady = ready;
	unlockReady();
	return isReady;
}
bool Driver::gameFull() const {
	return namesSz() >= DEFAULT_NUM_SEATS;
}
bool Driver::gameOver() const {
	return over;
}
void Driver::setReady() {
	lockReady();
	ready = true;
	unlockReady();
}
void Driver::setOver(bool isOver) {
	over = isOver;
}
#pragma endregion

/*
* Display a given message at socket for each player
*/
#pragma region Display
void Driver::display(const char msg[]) const {
	for (const auto mapping : map) {
		writeTo(mapping.second, msg, strlen(msg));
	}
}
void Driver::display(ROUND_STATUS status) const {
	for (const auto mapping : map) {
		writeTo(mapping.second, status);
	}
}
void Driver::displayCards() const {
	display(game->displayCards().c_str());
	wait(&start, &end, Default::DISPLAY_WAIT);
}
void Driver::displayTurn(const std::string* username) const {
	display(username->c_str());
}
void Driver::displayStats() const {
	display(game->printStats().c_str());
	wait(&start, &end, Default::DISPLAY_WAIT * 2);
}
#pragma endregion

/*
* Construct driver with username and game id to pass to Game,
* destruct driver by deallocating Game memory
*/
#pragma region Constructor, Destructor

Driver::Driver(const NewPlayer* player)  :id{player->gameId},
										  game{new Game{player->userName, id}},
										  ready{false}, over{false} {
	initAllMtx();
	addToNames(&player->userName); 
	addToMap(&player->userName, player->sock);
}
Driver::~Driver() {
	delete game;
}
#pragma endregion
