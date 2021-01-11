/*
* Driver.h
*
* CSS 432 Group Lucky No 13
*/

#ifndef DRIVER_H
#define DRIVER_H

#include <set>
#include <queue>
#include "Utilities.h"
#include "Game.h"


class Driver {

	/*
	* Mutexes for thread safety on shared resources
	*/
	mutable pthread_mutex_t namesMtx, qMtx, readyMtx;

	/*
	* Set of player names for new player threads to access without
	* interrupting the map
	*/
	std::set<std::string> names;

	/*
	* For other threads to add new players who will wait
	* until the beggining of next round
	*/
	std::queue<PlayerMapping> q;

	/*
	* Map active player's username to socket file descriptor
	*/
	std::map<std::string, int> map;

	/*
	* Queue for players quitting in game
	*/
	std::queue<PlayerMapping> quitQ;

	/*
	* BlackJack game to drive with id,
	* ready status and over status
	*/
	const int id;
	Game* game;
	bool ready, full, over;

	/*
	* Timers for pausing thread execution to let players see
	* displayed information before next round
	*/
	mutable timeval start, end;

	/*
	* Display the given message at socket for each player
	*/
	void display(const char msg[]) const;
	void display(ROUND_STATUS status) const;
	void displayTurn(const std::string* username) const;
	void displayCards() const;
	void displayStats() const;

	/*
	* Return true if q is emtpy
	*/
	bool qEmpty() const;

	/*
	* Add player to active state
	*/
	void addActivePlayer(const PlayerMapping*);

	/*
	* Add all waiting players to active map
	*/
	void addWaitingPlayers();

	/*
	* Send signal to all players
	*/
	void signal(bool) const;

	/*
	* Check if players quitted at betting
	*/
	void checkQuits();

	/*
	* Have each player place their bet
	*/
	void placeBets();

	/*
	* Check status for round after betting
	*/
	ROUND_STATUS checkStatus() const;
	ROUND_STATUS getStatus() const;

	/*
	* Read the players action from socket
	*/
	static ACTION readAction(int sock);
	
	/*
	* Player quits game
	*/
	void quit(const PlayerMapping*);

	/*
	* Switch state for given action
	*/
	bool switchAction(stringstream*, const std::string* username, ACTION);

	/*
	* Do the player's given action
	*/
	bool doAction(const PlayerMapping*, ACTION);

	/*
	* Signal next turn is starting
	*/
	void signalTurn() const;

	/*
	* Signal player's turns are over
	*/
	void signalTurnsOver() const;

	/*
	* Given player takes their turn
	*/
	void takeTurn(PlayerMapping* mapping);

	/*
	* Have each player take their turn
	*/
	void takeTurns();

	/*
	* Dealer actions
	*/
	void dealer();

	/*
	* Check if another round to go
	*/
	void checkIfNextRound();

	/*
	* Remove the player with the given username
	*/
	void removePlayer(const PlayerMapping*);
	

	/*
	* Initialization, locks and unlocks on mutexes for resources
	* used by multiple threads
	*/
	void initAllMtx() const;
	void lockNames() const;
	void unlockNames() const;
	void lockQ() const;
	void unlockQ() const;
	void lockReady() const;
	void unlockReady() const;

	/*
	* Add to / remove from resources used by multiple threads,
	* requires locking and unlocking mutexes
	*/
	void addToNames(const std::string* username);
	void addToQ(const std::string* username, int sock);
	void addToMap(const std::string* username, int sock);
	void addToMap(const PlayerMapping*);
	void removeFromNames(const std::string* username);
	void removeFromMap(const std::string* username);

	/*
	* Return amount of names/players
	*/
	int namesSz() const;

	/*
	* Set ready to true once there are at least two players
	*/
	void setReady();

	/*
	* Set the over signal for server thread to know if game should continue
	*/
	void setOver(bool);

public:

	/*
	* Create driver with username and game id to pass to Game
	*/
	explicit Driver(const NewPlayer*);

	/*
	* Deallocate the Game pointer
	*/
	~Driver();

	/*
	* Checking Game's ID, ready, full or over status
	*/
	int gameId() const;
	bool gameReady() const;
	bool gameFull() const;
	bool gameOver() const;

	/*
	* Return true if user name taken in game
	*/
	bool hasUserName(const std::string* userName) const;

	/*
	* Return all user names on seperate lines in a single string
	*/
	std::string userNameList() const;

	/*
	* Return true if can add player in game
	*/
	bool addPlayer(const NewPlayer*);

	/*
	* Begin a round in the Game
	*/
	void startRound();

};
#endif
