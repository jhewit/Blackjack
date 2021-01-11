//-----------------------------------Seat.cpp-----------------------------------
//Author: Johnathan Hewit
//Team: LuckyNo13
//Description: This is the implementation file for the Seat class. It contains the
//             Player assigned to the Seat, their status (active or inactive),
//             whether or not they've BUST or Surrendered, and the amount of
//             of their bet as private members, and exposes functions to
//             retrieve and modify those attributes. The Seat class is used
//             within Game and is created by the Game class.
//------------------------------------------------------------------------------
#include "Seat.h"

//------------------------------------Seat--------------------------------------
//Description: Default (empty) constructor for a Seat.
//Parameters:  - N/A
//------------------------------------------------------------------------------
Seat::Seat()
{
    this->playerSeated = nullptr;
    this->seatOccupied = false;
    this->bust = false;
    this->betAmount = 0;
} // end of Seat

//------------------------------------Seat--------------------------------------
//Description: This is a full constructor and can only be used when the Seat is
//             is being filled at the beginning of a game.
//Parameters:  - N/A
//------------------------------------------------------------------------------
Seat::Seat(Player* player)
{
    seatPlayer(player);
} // end of Seat

//-----------------------------------~Seat--------------------------------------
//Description: Destructor for a Seat.
//Parameters:  - N/A
//------------------------------------------------------------------------------
Seat::~Seat()
{
    if (seatOccupied)
    {
        unseatPlayer();
    }
} // end of ~Seat

//----------------------------------seatPlayer----------------------------------
//Description: Public function that seats a player in this Seat and sets that
//             Player's status.
//Parameters:  - player: The Player seated.
//             - playerStatus: Flag for whether or not the Player is active
//                             (eligible to play).
//------------------------------------------------------------------------------
bool Seat::seatPlayer(Player* player)
{
    if (!this->seatOccupied)
    {
        this->playerSeated = player;
        this->seatOccupied = true;
        return true;
    }
    return false;
} // end of seatPlayer

//--------------------------------unseatPlayer----------------------------------
//Description: Public function that removes the seated Player from the Seat.
//Parameters:  - N/A
//------------------------------------------------------------------------------
bool Seat::unseatPlayer()
{
    if (this->seatOccupied)
    {
        delete playerSeated;
        this->seatOccupied = false;
        return true;
    }
    return false;
} // end of unseatPlayer

//----------------------------------isSeated------------------------------------
//Description: Public function to return the flag of whether or not the Seat
//             is occupied.
//Parameters:  - N/A
//------------------------------------------------------------------------------
bool Seat::isSeated() const
{
    return this->seatOccupied;
} // end of isSeated

//-----------------------------------hasBust------------------------------------
//Description: Public function to return the flag of whether or not the Player
//             has Bust.
//Parameters:  - N/A
//------------------------------------------------------------------------------
bool Seat::hasBust() const
{
    return bust;
} // end of hasBust

//-----------------------------------setBust------------------------------------
//Description: Public function to set the flag of whether or not the Player
//             has Bust.
//Parameters:  - busted: Flag for whether or not the Player has Bust.
//------------------------------------------------------------------------------
bool Seat::setBust(bool busted)
{
    this->bust = busted;
} // end of setBust

//---------------------------------getPlayer------------------------------------
//Description: Public function to expose a pointer for the Player object in this
//             Seat.
//Parameters:  - N/A
//------------------------------------------------------------------------------
Player* Seat::getPlayer() const
{
    return this->playerSeated;
} // end of getPlayer

//-----------------------------------getBet-------------------------------------
//Description: Public function to return the amount of the bet of the Player.
//Parameters:  - N/A
//------------------------------------------------------------------------------
int Seat::getBet() const
{
    return this->betAmount;
} // end of getBet

//-----------------------------------setBet-------------------------------------
//Description: Public function to set the amount of the bet for the Player.
//Parameters:  - amount: The amount of the bet.
//------------------------------------------------------------------------------
bool Seat::setBet(int amount)
{
    this->betAmount = amount;
    return true;
} // end of setBet

//--------------------------------hasSurrendered--------------------------------
//Description: Public function to return the flag for whether or not the Player
//             has Surrended.
//Parameters:  - N/A
//------------------------------------------------------------------------------
bool Seat::hasSurrended() const
{
    return this->surrendered;
} // end of hasSurrendered

//----------------------------------setSurrender--------------------------------
//Description: Public function to set the flag for whether or not the Player
//             has Surrended.
//Parameters:  - N/A
//------------------------------------------------------------------------------
bool Seat::setSurrender(bool status)
{
    this->surrendered = status;
    return true;
} // end of setSurrender

//--------------------------------operator<<------------------------------------
//Description: Function to overload the output operator to return information
//             about the Seat.
//Parameters:  - output: The output ostream object.
//             - seat:   The Seat object that will be printed.
//------------------------------------------------------------------------------
ostream &operator<<(ostream &output, const Seat &seat)
{
    if (seat.seatOccupied)
    {
        output << *seat.getPlayer();
    }
    else
    {
        output << "Seat is empty.";
    }
    return output;
} // end of operator<<