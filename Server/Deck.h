//-----------------------------------Deck.h-------------------------------------
//Author: Johnathan Hewit
//Team: LuckyNo13
//Description: This is the header file for the Deck Class. It contains a vector
//             for temporary storage of Card objects as they are created (cards),
//             a stack for the Deck of Cards after they are shuffled, the number
//             of total cards, and a function shuffle the Cards as private members.
//             It exposes functions to retrieve and adjust those attributes. The 
//             class is used within Game and is created by the Game class.
//------------------------------------------------------------------------------
#ifndef DECK_H
#define DECK_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <stack>
#include <random>
#include <chrono>
#include <algorithm>
#include "card.h"
using namespace std;
                              //Hearts   Spades    Diamonds  Clubs
static const string SUITS[] = {"\u2665", "\u2660", "\u2666", "\u2663"};
static const map<string, int> cardValues = {{"2", 2}, {"3", 3}, {"3", 4},
                        {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9},
                        {"10", 10}, {"J", 10}, {"Q", 10}, {"K", 10}, {"A", 11}};

class Deck
{
  private:
    vector<Card*> cards;  // Temporary vector to hold cards after being created for shuffling
    stack<Card*> deck;    // Stack to hold the Deck
    int totalCards;       // Total number of Cards in the Deck
    void shuffleDeck();   // Function to shuffle the Deck

  public:
    // Constructors
    Deck(const int &);
    ~Deck();
    // Getters
    int getTotalCards() const;
    void printContents() const;
    // Setters
    bool addCard(Card *);
    Card* drawCard(bool visible);
    void makeDeck();
    void emptyDeck();
};
#endif
