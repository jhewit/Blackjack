//---------------------------------Game.h---------------------------------------
//Author: Johnathan Hewit
//Team: LuckyNo13
//Description: This is the header file for the Game class. It is the controller
//             class that aggregates all other objects in the game. It contains
//             a vector for Seats, a Deck called a shoe that contains all the 
//             Card objects currently in play, a flag for whether or not the
//             round is active, values for the game's ID, how many total Players
//             and active Players, the round number, and private functions to
//             discard the dealer's hand, update the dealer's Cards point values,
//             and initialize the Seats when the game starts. It exposes functions
//             to retrieve and modify those attributes. The class is used within
//             a driver file and is created by a driver.
//------------------------------------------------------------------------------
#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <unistd.h>
#include <iomanip>
#include "Deck.h"
#include "Player.h"
#include "card.h"
#include "Seat.h"
using namespace std;

static const int DEFAULT_DECK_SIZE = 4; //Number of decks of cards
static const int DEFAULT_NUM_SEATS = 4; //Number of seats at table
static const int MINIMUM_BET = 2;       //Minimum dollar amount for bets
static const int MAXIMUM_BET = 10;      //Maximum dollar amount for bets

class Game
{
    private:
        Seat* seats[DEFAULT_NUM_SEATS];     //Array for seat objects holding players
        vector<Card*> dealerHand;           //Vector containing dealer's current hand
        Deck *shoe;                         //A cumulative deck for the shoe (card dispenser)
        int dealerPoints;                   //The sum of points of the card values for dealer
        int gameID;                         //The ID number for the game
        int totalPlayers;                   //Number of total players in the game
        int scores[4];                      //Array for keeping scores of players
        int roundNum;                       //The number of the current round
        bool discardHand();
        void initializeSeats();
        void updateDealerCards();
        bool dealCards();

    public:
        Game(string username, const int gameID);
        ~Game();
        //Getters
        int getGameID() const;
        string displayCards() const;
        string printStats() const;

        //Setters
        bool removePlayer(string username);
        bool addPlayer(string username);
        void startRound();
        int placeBet(string username, int amount);
        bool hit(string username);
        bool surrender(string username);
        bool doubleDown(string username);
        bool bustCheck(string username, bool dealer);
        bool dealerActions();
        bool settleBets();

};
#endif
