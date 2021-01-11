//---------------------------------Game.cpp-------------------------------------
//Author: Johnathan Hewit
//Team: LuckyNo13
//Description: This is the implementation file for the Game class. It is the controller
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
#include "Game.h"

//------------------------------------Game--------------------------------------
//Description: Constructor for Game. Creates a new instance of a game when
//             receiving a username and a game ID.
//Parameters: - username:   Player who created the game.
//            - gameID:     Identifier for this game.
//------------------------------------------------------------------------------
Game::Game(string username, const int gameID)
{
    this->gameID = gameID;
    this->totalPlayers = 0;
    this->dealerPoints = 0;
    this->roundNum = 0;
    this->shoe = new Deck(DEFAULT_DECK_SIZE);
    initializeSeats();
    addPlayer(username);
} // end of Game

//------------------------------------~Game-------------------------------------
//Description: Deconstructor for Game. Clears memory created for the shoe and
//             seats.
//Parameters:  N/A
//------------------------------------------------------------------------------
Game::~Game()
{
    delete shoe;
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        delete seats[i];
    }
} // end of ~Game

//--------------------------------removePlayer----------------------------------
//Description: Public function that removes a specified player from the game.
//Parameters:  - username: Specified player.
//------------------------------------------------------------------------------
bool Game::removePlayer(string username)
{
    if (totalPlayers == 0)
    {
        return false;
    }
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        if (seats[i]->isSeated())
        {
            if (this->seats[i]->getPlayer()->getName() == username)
            {
                this->seats[i]->unseatPlayer();
                this->totalPlayers--;
                return true;
            }
        }
    }
    return false;
} // end of removePlayer

//----------------------------------addPlayer-----------------------------------
//Description: Public function that adds a specified player to the game.
//Parameters:  - username: Specified player.
//------------------------------------------------------------------------------
bool Game::addPlayer(string username)
{
    if (totalPlayers == 4) //Check if at max numbers of players
    {
        return false;
    }
    Player *newPlayer = new Player(username);
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++) //Otherwise, create and seat player
    {
        if (!seats[i]->isSeated()) //Empty seat
        {
            seats[i]->seatPlayer(newPlayer);
            this->totalPlayers++;
            return true;
        }
    }
    return false;
} // end of addPlayer

//-------------------------------initializeSeats--------------------------------
//Description: Private function that creates all the new seat objects.
//Parameters:  N/A
//------------------------------------------------------------------------------
void Game::initializeSeats()
{
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        Seat *newSeat = new Seat();
        this->seats[i] = newSeat;
    }
} // end of initializeSeats

//------------------------------updateDealerCards-------------------------------
//Description: Private function that updates the dealer's card values when the
//             dealer's hand is saved from BUST by changing the value of an Ace.
//Parameters:  N/A
//------------------------------------------------------------------------------
void Game::updateDealerCards()
{
    this->dealerPoints = 0;
    for (int i = 0; i < this->dealerHand.size(); i++)
    {
        this->dealerPoints += dealerHand[i]->getValue();
    }
} // end of updateDealerCards

//----------------------------------bustCheck-----------------------------------
//Description: Public function that checks if the player or dealer has Bust.
//             Receives a username of the player and a boolean if the check is
//             for a dealer instead of a player. Returns true if the player or
//             dealer has Bust, and false if not.
//Parameters:  - username: Player for whom the bust check will occur.
//             - dealer:   Flag indicating whether or not the dealer is being
//                         checked; true = yes, false = no.
//------------------------------------------------------------------------------
bool Game::bustCheck(string username, bool dealer)
{
    if (username == "dealer" && dealer == true)
    {
        if (dealerPoints > 21)
        {
            for (int i = 0; i < dealerHand.size(); i++)
            {
                if (dealerHand[i]->getFace() == "A" && dealerHand[i]->getValue() == 11)
                {
                    dealerHand[i]->setValue(1);
                    updateDealerCards();
                    return false;
                }
            }
            return true;
        }
    }
    else
    {
        for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
        {
            if (seats[i]->isSeated())
            {
                if (seats[i]->getPlayer()->getName() == username)
                {
                    if (seats[i]->getPlayer()->getPoints() > 21)
                    {
                        vector<Card*> tempHand(seats[i]->getPlayer()->getHand());
                        for (int j = 0; j < tempHand.size(); j++)
                        {
                            if (tempHand[j]->getFace() == "A" && tempHand[j]->getValue() == 11)
                            {
                                seats[i]->getPlayer()->getHand()[j]->setValue(1);
                                seats[i]->getPlayer()->updateCardValues();
                                return false;
                            }
                        }
                        return true;
                    }
                }
            }
        }
    }
    return false;
} // end of bustCheck

//----------------------------------startRound----------------------------------
//Description: Public function to set round-specific variables to default values
//             and deal the cards.
//Parameters:  N/A
//------------------------------------------------------------------------------
void Game::startRound()
{
    this->roundNum++;
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++) // Reset all round and turn variables/states
    {
        seats[i]->setBust(false);
        seats[i]->setSurrender(false);
    }
    dealCards();
} // end of startRound

//----------------------------------dealCards-----------------------------------
//Description: Private function to deal the cards once a round has begun. Also
//             checks if the total number of cards is less than a single deck
//             and discards remaining cards and creates a new deck.
//Parameters:  N/A
//------------------------------------------------------------------------------
bool Game::dealCards()
{
    if (totalPlayers < 2)
    {
        cerr << "Unable to deal cards. There are too few active players in the game." << endl;
        return false;
    }
    if (this->shoe->getTotalCards() <= 52) //If there is one deck or less in the shoe
    {
        cerr << "Dealer:\nLow on cards... emptying shoe and dealing another " << DEFAULT_DECK_SIZE << " decks.\n" << endl;
        this->shoe->emptyDeck();
        for (int i = 0; i < DEFAULT_DECK_SIZE; i++)
        {
            this->shoe->makeDeck();
        }
    }
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        if (seats[i]->isSeated()) //added
        {
            this->seats[i]->getPlayer()->addCard(this->shoe->drawCard(true));
            this->seats[i]->getPlayer()->addCard(this->shoe->drawCard(true));
            if (bustCheck(seats[i]->getPlayer()->getName(), false) == true)
            {
                seats[i]->setBust(true);
            }
        }
    }
    this->dealerHand.push_back(this->shoe->drawCard(true));
    this->dealerHand.push_back(this->shoe->drawCard(false));
    bustCheck("dealer", true);
} // end of dealCards

//----------------------------------placeBet------------------------------------
//Description: Public function that allows players to place their bet at the
//             beginning of the round. Returns the amount of the bet if the
//             bet is successful and permitted. If the amount of the bet exceeds
//             the Player's balance, their balance is returned. In any other
//             situation, -1 is returned for failure.
//Parameters:  - username: Player who is placing the bet.
//             - amount:   The amount of the bet.
//------------------------------------------------------------------------------
int Game::placeBet(string username, int amount)
{
    if (amount < MINIMUM_BET || amount > MAXIMUM_BET)
    {
        return -1;
    }
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        if (seats[i]->isSeated())
        {
            if (seats[i]->getPlayer()->getName() == username)
            {
                if (seats[i]->getPlayer()->getBalance() - amount < 0)
                {
                    return seats[i]->getPlayer()->getBalance();
                } 
                seats[i]->setBet(amount);
                return amount;
            }
        }
    }
    return -1;
} // end of placeBet

//-------------------------------------hit--------------------------------------
//Description: Public function for a player to hit, or get dealt an additional
//             card.
//Parameters:  - username: The player who is receiving the card.
//------------------------------------------------------------------------------
bool Game::hit(string username)
{
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        if (seats[i]->isSeated())
        {
            if (seats[i]->getPlayer()->getName() == username)
            {
                if (seats[i]->hasBust() == true)
                {
                    return false;
                }
                this->seats[i]->getPlayer()->addCard(this->shoe->drawCard(true));
                if (bustCheck(username, false) == true)
                {
                    seats[i]->setBust(true);
                    return false;
                }
            }
        }
    }
    return true;
} // end of hit

//----------------------------------surrender-----------------------------------
//Description: Public function to surrender the player's hand.
//Parameters:  - username: Player who is surrendering.
//------------------------------------------------------------------------------
bool Game::surrender(string username)
{
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        if (seats[i]->isSeated())
        {
            if (seats[i]->getPlayer()->getName() == username)
            {
                seats[i]->setSurrender(true);
                seats[i]->getPlayer()->discardHand();
            }
        }
    }
} // end of surrender

//---------------------------------doubleDown-----------------------------------
//Description: Public function to double down -- in other words, the player
//             doubles their bet and only receives one additional card.
//Parameters:  - username: Player who is doubling down.
//------------------------------------------------------------------------------
bool Game::doubleDown(string username)
{
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        if (seats[i]->isSeated())
        {
            if (seats[i]->getPlayer()->getName() == username)
            {
                if (seats[i]->getPlayer()->getBalance() - (seats[i]->getBet() * 2) < 0)
                {
                    this->seats[i]->setBet(seats[i]->getPlayer()->getBalance());
                    if (bustCheck(username, false) == true)
                    {
                        this->seats[i]->setBust(true);
                    }
                    return true;
                }
                else
                {
                    seats[i]->setBet(seats[i]->getBet() * 2);
                    this->seats[i]->getPlayer()->addCard(this->shoe->drawCard(true));
                    if (bustCheck(username, false) == true)
                    {
                        this->seats[i]->setBust(true);
                    }
                    return true;
                }
            }
        }
    }
    return false;
} // end of doubleDown

//-------------------------------dealerActions----------------------------------
//Description: Public fuunction to trigger dealer behavior of checking if players
//             have bust or if the dealer drew a natural or a blackjack. If not,
//             or if the dealer has less than 17 cards, it will hit until it doesn't.
//Parameters:  N/A
//------------------------------------------------------------------------------
bool Game::dealerActions()
{
    //Flip invisible card
    this->dealerHand[1]->flip();
    //Assess dealer card values
    for (int i = 0; i < dealerHand.size(); i++)
    {
        dealerPoints += dealerHand[i]->getValue();
    }
    //Check if all players have bust
    int bustCount = 0;
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        if (seats[i]->isSeated() == true && seats[i]->hasBust() == true)
        {
            bustCount++;
        }
    }
    if (dealerPoints == 21)
    { //Blackjack -- Settle bets and end round
        return true;
    }
    else
    {
        if (bustCount == totalPlayers)
        { //If all Players have bust, don't bother hitting
            return true;
        } 
        while (dealerPoints < 17) //Hit
        { //Keep hitting until point value of cards is >= 17
            this->dealerHand.push_back(this->shoe->drawCard(true));
            this->dealerPoints += this->dealerHand[dealerHand.size() - 1]->getValue();
            if (bustCheck("dealer", true) == true)
            { //If the dealer busts, settle up
                return true;
            }
        }
    }
    // End Round & Display
    return true;
} // end of dealerActions

//--------------------------------settleBets------------------------------------
//Description: Public function to settle the bets after the dealer has finished
//             its turn and the round has concluded.
//Parameters:  N/A
//------------------------------------------------------------------------------
bool Game::settleBets()
{
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        if (this->seats[i]->isSeated())
        {
            if (this->seats[i]->getPlayer()->getPoints() == this->dealerPoints && this->seats[i]->hasBust() == false)
            { // Push -- player gets back their bet
                //Do nothing, player keeps their bet, set bet to 0, leave balance alone
            }
            else if ((this->seats[i]->getPlayer()->getPoints() > this->dealerPoints && this->seats[i]->hasBust() == false) || this->dealerPoints > 21 && this->seats[i]->hasBust() == false)
            { // Player wins, pay out bet amount to balance
                //Pay out player the amount of their bet, set bet to 0, add bet amount to balance
                seats[i]->getPlayer()->addBalance(seats[i]->getBet());
                seats[i]->getPlayer()->addWin();
            }
            else if (this->seats[i]->hasSurrended() == true)
            { // Player surrenders and loses half of their bet
                seats[i]->getPlayer()->removeBalance(seats[i]->getBet()/2);
                seats[i]->getPlayer()->addLoss();
            }
            else
            { // Player has bust or has less points than the dealer and loses their bet from balance
                //Player loses their bet, set bet to 0, subtract bet amount from balance
                seats[i]->getPlayer()->removeBalance(seats[i]->getBet());
                seats[i]->getPlayer()->addLoss();
            }
            seats[i]->getPlayer()->discardHand(); //Discard cards in hand; reset points to 0
            seats[i]->setBet(0);
        }
    }
    discardHand();
} // end of settleBets

//--------------------------------discardHand-----------------------------------
//Description: Private function to discard remaining cards from dealer's hand
//             once the round has concluded.
//Parameters:  N/A
//------------------------------------------------------------------------------
bool Game::discardHand()
{
  if (!dealerHand.empty())
  {
    for (int i = 0; i < dealerHand.size(); i++)
    {
      delete dealerHand[i];
      dealerHand[i] = nullptr;
    }
    dealerHand.clear();
  }
  this->dealerPoints = 0;
} // end of discardHand

//--------------------------------displayCards----------------------------------
//Description: Public function to display the cards of every player active in the
//             game as well as the dealer.
//Parameters:  N/A
//------------------------------------------------------------------------------
string Game::displayCards() const
{
    stringstream ss;
    ss << "\t\t      Hands" << endl;
    ss << "___________________________________________________" << endl;
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        if (seats[i]->isSeated())
        {
            ss << seats[i]->getPlayer()->getName() << " (Bet: $" << seats[i]->getBet() << ") " << endl;
            if (seats[i]->isSeated() == true)
            {
                vector<Card*> tempHand(seats[i]->getPlayer()->getHand());
                for (int j = 0; j < tempHand.size(); j++)
                {
                    ss << *tempHand[j] << " ";
                }
                if (seats[i]->hasBust() == true)
                {
                    ss << "-- BUST";
                }
                if (seats[i]->getPlayer()->getPoints() == 21)
                {
                    ss << "-- BLACKJACK";
                }
                if (seats[i]->hasSurrended() == true)
                {
                    ss << "SURRENDERED";
                }
                ss << "\n" << endl;
            }
        }
    }
    ss << "Dealer" << endl;
    for (int k = 0; k < dealerHand.size(); k++)
    {
        if (dealerHand[k]->isVisible() == false)
        {
            ss << "[    ]";
        }
        else
        {
            ss << *dealerHand[k] << " ";   
        }
    }
    if (dealerPoints > 21)
    {
        ss << "-- BUST";
    }
    if (dealerPoints == 21)
    {
        ss << "-- BLACKJACK";
    }
    ss << "\n___________________________________________________\n" << endl;
    return ss.str();
} // end of displayCards

//---------------------------------printStats-----------------------------------
//Description: Public function to print the scoreboard; generally done at the
//             end of a round.
//Parameters:  N/A
//------------------------------------------------------------------------------
string Game::printStats() const
{
    stringstream ss;
    ss << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    if (roundNum == 0)
    {
        ss << "|" << setw(48) << "Starting Round 1" << setw(33) << "|\n";
    }
    else
    {
        ss << "|" << setw(40) << "Round " << this->roundNum << setw(40) << "|\n";
    }
    ss << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    ss << "|" << setw(16) << "Player" << setw(12) << "|" << setw(12) << "Balance" << setw(6); 
    ss << "|" << setw(10) << "Wins"<< setw(7) << "|" << setw(11) << "Losses" << setw(7) << "|\n";
    ss << "---------------------------------------------------------------------------------\n";
    for (int i = 0; i < DEFAULT_NUM_SEATS; i++)
    {
        if (seats[i]->isSeated() == true)
        {
            ss << *seats[i]->getPlayer();
        }
    }
    ss << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    return ss.str();
} // end of printStats