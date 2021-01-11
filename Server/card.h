//-----------------------------------Card.h-------------------------------------
//Author: Johnathan Hewit
//Team: LuckyNo13
//Description: This is the header file for the Card Class. It contains the value,
//             suit, face type, and a flag for whether or not the card is visible
//             as private members, and exposes functions to retrieve and adjust those
//             attributes. The class is used within Decks and Players/Dealer hands
//             and is created by the Deck class.
//------------------------------------------------------------------------------
#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Card
{
    friend ostream &operator<<(ostream &, const Card &);
    private:
      string face;        // Face value of a card (e.g: Ace or 2)
      int pointValue;     // Points value of a card
      string suit;        // Suit value of a card (e.g: Hearts or Spades)
      bool visible;       // Whether or not the card is visible

    public:
      // Constructors
      Card(const string &suit, const string &face, const int &pointValue);
      Card(const Card &card);
      ~Card();
      // Getters
      int getValue() const;
      string getFace() const;
      string getSuit() const;
      bool isVisible() const;
      // Setters
      void flip();
      bool setValue(int value);
};
#endif
