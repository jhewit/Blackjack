//---------------------------------Card.cpp-------------------------------------
//Author: Johnathan Hewit
//Team: LuckyNo13
//Description: This is the implentation file for the Card Class. It contains the value,
//             suit, face type, and a flag for whether or not the card is visible
//             as private members, and exposes functions to retrieve and adjust those
//             attributes. The class is used within Decks and Players/Dealer hands
//             and is created by the Deck class.
//------------------------------------------------------------------------------
#include "card.h"

//---------------------------------Card()---------------------------------------
//Description: Default constructor for a Card.
//Parameters:  - suit: Suit of the card (e.g: Hearts, Spades).
//             - face: Face of the card (e.g: Ace, 2).
//             - pointValue: Point value of the incoming card.
//------------------------------------------------------------------------------
Card::Card(const string &suit, const string &face, const int &pointValue)
{
  this->suit = suit;
  this->face = face;
  this->pointValue = pointValue;
  visible = false;
} // end of Card()

//---------------------------------Card()---------------------------------------
//Description: Copy constructor for a Card.
//Parameters:  - card: The card that we're copying.
//------------------------------------------------------------------------------
Card::Card(const Card &card)
{
  this->suit = card.getSuit();
  this->face = card.getFace();
  this->pointValue = card.getValue();
  this->visible = card.isVisible();
} // end of Card()

//--------------------------------~Card()---------------------------------------
//Description: Destructor for a Card.
//Parameters:  N/A
//------------------------------------------------------------------------------
Card::~Card()
{} // end of ~Card()

//--------------------------------getValue--------------------------------------
//Description: Returns the point value for the card.
//Parameters:  N/A
//------------------------------------------------------------------------------
int Card::getValue() const
{
  return this->pointValue;
} // end of getValue

//--------------------------------getFace---------------------------------------
//Description: Returns the face for the card.
//Parameters:  N/A
//------------------------------------------------------------------------------
string Card::getFace() const
{
  return this->face;
} // end of getFace

//--------------------------------getSuit---------------------------------------
//Description: Returns the suit for the card.
//Parameters:  N/A
//------------------------------------------------------------------------------
string Card::getSuit() const
{
  return this->suit;
} // end of getSuit

//------------------------------isVisible---------------------------------------
//Description: Returns whether or not card is visible.
//Parameters:  N/A
//------------------------------------------------------------------------------
bool Card::isVisible() const
{
  return this->visible;
} // end isVisible

//---------------------------------flip-----------------------------------------
//Description: Flips the card over making visible hidden and vise versa.
//Parameters:  N/A
//------------------------------------------------------------------------------
void Card::flip()
{
  if (this->visible)
  {
    this->visible = false;
  }
  else
  {
    this->visible = true;
  }
} // end of flip

//-------------------------------setValue---------------------------------------
//Description: Flips the card over making visible hidden and vise versa.
//Parameters:  - value: The new value of the card; only applies to the Ace.
//------------------------------------------------------------------------------
bool Card::setValue(int value)
{
  if (this->face != "A")
  {
    return false;
  }
  this->pointValue = value;
} // end of setValue

//-------------------------------operator<<-------------------------------------
//Description: Output operator overload to print representation of the Card.
//Parameters:  N/A
//------------------------------------------------------------------------------
ostream &operator<<(ostream &output, const Card &card)
{
  return (output << "[ " << card.face << card.suit << " ]");
} // end of operator<<
