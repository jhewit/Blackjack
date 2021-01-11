# Poker
This is the final project for the CSS432 Computer Networking course at the University of Washington. It functions as a multiplayer game of Poker for up to 4 concurrent players who play against the dealer, a non-playing character. It is a C++ project that is designed where the game of Poker itself is run on a server application that listens for incoming packets from the client applications to establish a game, and communicate the various stages or phases of a game of Poker. My contribution for this project was in the game mechanics and core functionality found in the Server package. In addition to myself, there was one additional contributor who focused on the Client package for the project.

## Requirements
- Linux environment

## Instructions for running
1. Copy the Server package/folder to the desired Linux environment location
2. Run `./build.sh` from the terminal/command line to compile the executable
3. Run `./server` from the terminal/command line to launch the server
4. Copy the Client package/folder to the desired Linux environment location
5. Run `./build.sh` from the terminal/command line to compile the executable
6. Run `./client <server_host_name>` from the terminal/command line to launch the client

## How to Play
1. Launch Server
2. Launch Client with the specified Server hostand make sure it connects.
3. When prompted for a username, type a name and press ‘enter’.
4. You are now in the main menu. Type ‘-1’to create a new gameor type the id of an existing game in the listto join it.The game will wait until at least two players join.
5. Once the game starts, you are prompted for a betbetween $2 and $10 dollars. If you wish to leave, type ‘-1’. Otherwise, type the bet amount.
6. Once all players have made bets, the cards are dealt out. One player can perform actions while the others must wait.
7. Once it is your turn, choose your desired action.
8. After all players have finished their turns, the dealer plays.
9. Finally, the round ends and the results are printed.
10. The game goes to the next round, and you are onceagain prompted for a bet.
11. To leave to main menu, type ‘-1’.
12. To leave thegame and unregister, type ‘-1’.
13. Close your Client.

*NOTE: Leaving the game with ‘CTRL-C’ will not properly unregister the player. If you leave and try to reconnect with the same username, it will not let you. In these cases, the game will need to be exited and re-entered.*

### Rules of Blackjack:
- Walk-in type game, where the player plays against the dealer only.
- Each player starts with two cards.
    - Dealer starts with one card facing down.
- Goal of the game is to have a higher sum of cards than the dealer. 
    - The maximum sum is 21, which is referred to as a Blackjack.
    - Anything over 21, you bust, at which point your only chance of drawing the game is if the dealer also busts.
- Card values:
    - All number cards are worth their listed amount (ex. 7 of hearts = 7 points)
    - All face cards are worth 10 (ex.king of diamonds= 10 points).
    - Aces are worth either 1 or 11, depending on whether 11 causes you to bust.
- Actions:
    - Hit: add another card.
    - Stand: end your turn.
    - Double down: double your bet and receive one more card (if you feel confident that one more card will be enough).This can only be done as your first action.
    - Surrender: get half of your bet back.
- Outcomes:
    - If you and the dealer tie (or both of you bust), you get your money back.
    - If you have a higher sum, you doubleyour bet.
    - If you have a lower sum, you lose your bet.

Detailed rules of Blackjack can be found here: http://www.hitorstand.net/strategy.php

To play the game, follow these steps: