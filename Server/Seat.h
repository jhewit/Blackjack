//-----------------------------------Seat.h-------------------------------------
//Author: Johnathan Hewit
//Team: LuckyNo13
//Description: This is the header file for the Seat class. It contains the
//             Player assigned to the Seat, their status (active or inactive),
//             whether or not they've BUST or Surrendered, and the amount of
//             of their bet as private members, and exposes functions to
//             retrieve and modify those attributes. The Seat class is used
//             within Game and is created by the Game class.
//------------------------------------------------------------------------------
#ifndef SEAT_H
#define SEAT_H
#include <string>
#include <vector>
#include <iostream>
#include "Player.h"
#include "card.h"
using namespace std;

class Seat
{
    friend ostream &operator<<(ostream &, const Seat &);
    private:
        Player* playerSeated;   //Player in the seat
        bool seatOccupied;      //Whether or not a player is seated
        bool surrendered;       //Whether or not the player has surrended
        bool bust;              //Whether or not the player has bust
        int betAmount;          //Amount of the current bet

    public:
        //Constructors
        Seat();
        Seat(Player*);
        ~Seat();
        //Getters
        bool isSeated() const;
        bool hasBust() const;
        bool hasSurrended() const;
        int getBet() const;
        Player* getPlayer() const;

        //Setters
        bool seatPlayer(Player*);
        bool unseatPlayer();
        bool setBust(bool busted);
        bool setBet(int amount);
        bool setSurrender(bool status);
};
#endif