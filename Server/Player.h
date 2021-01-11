//---------------------------------Player.h-------------------------------------
//Author: Johnathan Hewit
//Team: LuckyNo13
//Description: This is the header file for the Player Class. It contains the
//             username, balance in dollars, total points of the cards in the 
//             player's hand, the number of wins and losses, and a vector that
//             contains the player's cards for the given round as private members.
//             It exposes functions to retrieve and adjust those attributes. The 
//             class is used within Seats and is created by the Game object.
//------------------------------------------------------------------------------
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include "card.h"
using namespace std;

#define DEFAULT_BALANCE 500; // Default balance every player receives when joining a game

class Player
{
  friend ostream &operator<<(ostream &, const Player &);
  private:
    string username;        // Username of the Player
    int balance;            // Total balance of the Player's winnings/losses
    int points;             // Total points of all the cards in Player's hand
    int wins;               // Total wins for a Player
    int losses;             // Total losses for a Player
    vector<Card*> hand;     // Player's hand -- Vector of pointers of cards
  public:
    // Constructors
    Player(const string username);
    Player(const Player &player);
    ~Player();

    // Getters
    string getName() const;
    int getBalance() const;
    int getPoints() const;
    int getWins() const;
    int getLosses() const;
    vector<Card*> getHand() const;
    
    // Setters
    bool addBalance(int amount);
    bool removeBalance(int amount);
    bool addCard(Card*);
    bool discardHand();
    bool updateCardValues();
    bool addWin();
    bool addLoss();
};
#endif
