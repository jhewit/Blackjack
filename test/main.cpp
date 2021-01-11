#include "Deck.h"
#include "Player.h"
#include "Seat.h"
#include "Game.h"
#include <string.h>
#include <iostream>
#include <fcntl.h>

using namespace std;

int main ()
{
  //Test creating a game
  vector<string> players{"john", "vlad", "juan"};   // Players
  Game *newGame = new Game(players[0], 01);         // Creating game and passing first player
  newGame->addPlayer(players[1]);                   // Adding other two players
  newGame->addPlayer(players[2]);
  cout << "Welcome to Blackjack!\n\n" << endl;
  int turn = 0;               // Counter for whose turn it is
  string menu = "";
  string input = "";
  int bet;
  bool canHit = false;        // Flag for whether or not the Player can take its turn (i.e. If Player
  while (input != "quit")     // busts, surrenders, or stands, it'll be false.
  {
    input = "";
    newGame->startRound();
    for (int i = 0; i < players.size(); i++) // Bets
    {
      cout << players[i] << ", please enter your bet in whole dollars: ";
      cin >> bet;
      cout << endl;
      newGame->placeBet(players[i], bet);
    }
    cout << newGame->displayCards();
    for (turn = 0; turn < players.size(); turn++)
    {
      input = "";
      while (!(input == "stand" || input == "surrender" || input == "doubledown" || input == "quit")) // Turns
      {
        cout << "It's " << players[turn] << "'s turn!\n\nType 'hit', 'stand', 'surrender' or 'doubledown'\n" << endl; 
        cin >> input;
        if (input == "hit")
        {
          canHit = newGame->hit(players[turn]);
          cout << newGame->displayCards();
          if (canHit == false)
          {
            break;
          }
        }
        if (input == "surrender")
        {
          newGame->surrender(players[turn]);
          cout << newGame->displayCards();
          break;
        }
        if (input == "doubledown")
        {
          newGame->doubleDown(players[turn]);
          cout << newGame->displayCards();
          break;
        }
      }
    }
    newGame->dealerActions();
    cout << newGame->displayCards();
    newGame->settleBets();
    cout << newGame->printStats();
  }
  // //Set up a loop for game
  delete newGame;
  return 0;
}
