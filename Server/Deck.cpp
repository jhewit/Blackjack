//-----------------------------------Deck.cpp-----------------------------------
//Author: Johnathan Hewit
//Team: LuckyNo13
//Description: This is the header file for the Deck Class. It contains a vector
//             for temporary storage of Card objects as they are created (cards),
//             a stack for the Deck of Cards after they are shuffled, the number
//             of total cards, and a function shuffle the Cards as private members.
//             It exposes functions to retrieve and adjust those attributes. The 
//             class is used within Game and is created by the Game class.
//------------------------------------------------------------------------------
#include "Deck.h"

//----------------------------------Deck----------------------------------------
//Description: Default empty constructor for a Card.
//------------------------------------------------------------------------------
Deck::Deck(const int &numDecks)
{
  this->totalCards = 0;
  for (int i = 0; i < numDecks; i++)
  {
    makeDeck();
  }
} // end of Deck()

//----------------------------------~Deck---------------------------------------
//Description: Destructor for a Card.
//------------------------------------------------------------------------------
Deck::~Deck()
{
  emptyDeck();
} // end of ~Deck

//------------------------------getTotalCards-----------------------------------
//Description: Public function to return the total number of Cards.
//Parameters:  - N/A
//------------------------------------------------------------------------------
int Deck::getTotalCards() const
{
  return this->totalCards;
} // end of getTotalCards

//---------------------------------addCard--------------------------------------
//Description: Public function to add a Card to the Deck.
//Parameters:  - newCard: Pointer to a Card.
//------------------------------------------------------------------------------
bool Deck::addCard(Card *newCard)
{
  this->deck.push(newCard);
} // end of addCard

//--------------------------------drawCard--------------------------------------
//Description: Public function to return a pointer to a Card from the Deck and 
//             set whether or not the Card is visible.
//Parameters:  - visible: Flag for whether or not the Card is visible.
//------------------------------------------------------------------------------
Card* Deck::drawCard(bool visible)
{
  Card* copy = this->deck.top();
  if (visible == true)
  {
    copy->flip();
  }
  this->deck.pop();
  this->totalCards--;
  return copy;
} // end of drawCard

//---------------------------------makeDeck-------------------------------------
//Description: Public function to make the Deck. Creates the Cards and inserts
//             them into the temporary vector, shuffles them, and then inserts
//             them into the stack while updating the total number of Cards.
//Parameters:  - N/A
//------------------------------------------------------------------------------
void Deck::makeDeck()
{
  this->totalCards += 52;
  for (int i = 0; i < 4; i++)
  {
    for (auto itr = cardValues.begin(); itr != cardValues.end(); itr++)
    {
      Card* newCard = new Card(SUITS[i], itr->first, itr->second);
      this->cards.push_back(newCard);
    }
  }
  shuffleDeck();
  while(!this->cards.empty())
  {
    addCard(cards.back());
    cards.pop_back();
  }
} // end of makeDeck

//-------------------------------shuffleDeck------------------------------------
//Description: Private function to shuffle the Deck in the temporary vector.
//Parameters:  - N/A
//------------------------------------------------------------------------------
void Deck::shuffleDeck()
{
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  shuffle(this->cards.begin(), this->cards.end(), default_random_engine(seed));
} // end of shuffleDeck

//---------------------------------emptyDeck------------------------------------
//Description: Public function to empty the Deck of all Cards. Also deletes the 
//             Card objects from memory.
//Parameters:  - N/A
//------------------------------------------------------------------------------
void Deck::emptyDeck()
{
  if (!cards.empty())
  {
    for (int i = 0; i < cards.size(); i++)
    {
      delete cards[i];
      cards[i] = nullptr;
    }
  }
  while (!deck.empty())
  {
    Card* temp = this->deck.top();
    this->deck.pop();
    delete temp;
  }
  this->totalCards = 0;
}

//-----------------------------printContents------------------------------------
//Description: Public function to print the contents of the temporary vector of
//             Cards.
//Parameters:  - N/A
//------------------------------------------------------------------------------
void Deck::printContents() const
{
  for (int i = 0; i < cards.size(); i++)
  {
    cout << *cards[i] << " ";
  }
} // end of printContents
