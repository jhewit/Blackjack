//---------------------------------Player.cpp-----------------------------------
//Author: Johnathan Hewit
//Team: LuckyNo13
//Description: This is the implementation file for the Player Class. It contains the
//             username, balance in dollars, total points of the cards in the 
//             player's hand, the number of wins and losses, and a vector that
//             contains the player's cards for the given round as private members.
//             It exposes functions to retrieve and adjust those attributes. The 
//             class is used within Seats and is created by the Game object.
//------------------------------------------------------------------------------
#include "Player.h"

//----------------------------------Player--------------------------------------
//Description: Default constructor for Player.
//Parameters:  - username: The Player's username.
//------------------------------------------------------------------------------
Player::Player(const string username)
{
  this->username = username;
  this->balance = DEFAULT_BALANCE;
  this->points = 0;
  this->wins = 0;
  this->losses = 0;
} // end of Player

//----------------------------------Player--------------------------------------
//Description: Copy constructor for Player.
//Parameters:  - player: The Player that we're copying.
//------------------------------------------------------------------------------
Player::Player(const Player &player)
{
  this->username = player.getName();
  this->balance = player.getBalance();
} // end of Player

//---------------------------------~Player--------------------------------------
//Description: Destructor for Player.
//Parameters:  N/A
//------------------------------------------------------------------------------
Player::~Player()
{
  discardHand();
} // end of ~Player

//----------------------------------getName-------------------------------------
//Description: Returns the Player's username.
//Parameters:  N/A
//------------------------------------------------------------------------------
string Player::getName() const
{
  return this->username;
} // end of getName

//--------------------------------getBalance------------------------------------
//Description: Returns the Player's balance.
//Parameters:  N/A
//------------------------------------------------------------------------------
int Player::getBalance() const
{
  return this->balance;
} // end of getBalance

//---------------------------------getPoints------------------------------------
//Description: Returns the Player's total number of points based on the sum of 
//             card values in the Player's current hand.
//Parameters:  N/A
//------------------------------------------------------------------------------
int Player::getPoints() const
{
  return this->points;
} // end of getPoints

//---------------------------------getHand--------------------------------------
//Description: Returns the Player's hand (vector of Card pointers) back to the
//             calling function.
//Parameters:  N/A
//------------------------------------------------------------------------------
vector<Card*> Player::getHand() const
{
  return this->hand;
} // end of getCards

//---------------------------------addBalance-----------------------------------
//Description: Adds balance to Player's total balance. Called after a round
//             concludes and a Player wins. Positive numbers only.
//Parameters:  - amount: The amount to add to a Player's balance.
//------------------------------------------------------------------------------
bool Player::addBalance(int amount)
{
  if (amount >= 0)
  {
    this->balance += amount;
    return true;
  }
  return false;
} // end of addBalance

//-------------------------------removeBalance----------------------------------
//Description: Removes balance from Player's total balance. Called after a round
//             concludes and a PLayer loses. Positive numbers only.
//Parameters:  - amount: The amount to remove from Player's balance.
//------------------------------------------------------------------------------
bool Player::removeBalance(int amount)
{
  if ((this->balance - amount) <= 0 || amount < 0)
  {
    return false;
  }
  else
  {
    this->balance -= amount;
  }
} // end of removeBalance

//----------------------------------addWin--------------------------------------
//Description: Adds a win to the Player's total wins.
//Parameters:  N/A
//------------------------------------------------------------------------------
bool Player::addWin()
{
  this->wins++;
} // end of addWin

//----------------------------------getWins-------------------------------------
//Description: Returns the number of wins for a Player.
//Parameters:  N/A
//------------------------------------------------------------------------------
int Player::getWins() const
{
  return this->wins;
} // end of getWins

//----------------------------------addLoss-------------------------------------
//Description: Adds a loss to the PLayer's total losses.
//Parameters:  N/A
//------------------------------------------------------------------------------
bool Player::addLoss()
{
  this->losses++;
} // end of addLoss
 
//----------------------------------getLosses-----------------------------------
//Description: Returns the numebr of losses for a Player.
//Parameters:  N/A
//------------------------------------------------------------------------------
int Player::getLosses() const
{
  return this->losses;
} // end of getLosses

//----------------------------------addCard-------------------------------------
//Description: Adds a card to the Player's hand (vector of Card pointers).
//Parameters:  - newCard: A pointer for a card.
//------------------------------------------------------------------------------
bool Player::addCard(Card* newCard)
{
  this->hand.push_back(newCard);
  this->points += newCard->getValue();
  return true;
} // end of addCard

//--------------------------------discardHand-----------------------------------
//Description: Removes all cards from Player's hand and destroys the Card object.
//Parameters:  N/A
//------------------------------------------------------------------------------
bool Player::discardHand()
{
  if (!hand.empty())
  {
    for (int i = 0; i < hand.size(); i++)
    {
      delete hand[i];
      hand[i] = nullptr;
    }
    hand.clear();
  }
  this->points = 0;
} // end of discardHand

//------------------------------updateCardValues--------------------------------
//Description: Updates the values of the cards in the Player's hand. This is
//             called when a bustCheck in the Game object swaps the wild card
//             value of Ace from 11 to 1.
//Parameters:  N/A
//------------------------------------------------------------------------------
bool Player::updateCardValues()
{
  this->points = 0;
  for (int i = 0; i < hand.size(); i++)
  {
    this->points += hand[i]->getValue();
  }
} // end of updateCardValues

//--------------------------------operator<<------------------------------------
//Description: Output operator overload to print the Player's name, their balance,
//             and their numbers of wins and losses. Used by printStats in Game.
//Parameters:  - output: Output object.
//             - player: Player that will be printed.
//------------------------------------------------------------------------------
ostream &operator<<(ostream &output, const Player &player)
{
  output << setw(15) << player.getName() << setw(22) << "$" << player.getBalance();
  output << setw(16) << player.getWins() << setw(18) << player.getLosses() << "\n";
  return output;
}
