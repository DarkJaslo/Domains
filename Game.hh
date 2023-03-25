#ifndef Game_hh
#define Game_hh
#include "Player.hh"
#include "Utility.hh"
#include "Order.hh"
using namespace std;

class Game{

public:
  int board_cols() const;
  int board_rows() const;

private:
  int numPlayers;
  int unitsStart;
  int unitsMax;
  int unitsMin;
  int squaresMax;
  int bonusMax;
  int roundsPerRespawn;
  int pointsPerUnit;
  int pointsPerBubble;
  int pointsPerPop;
  int pointsPerSquare;
  int energyMax;
  int energyMin;
  int abilitySize;
  vector<Player*> players;
  vector<Order*> orders;
  vector<int> roundsSinceRespawn;

  Game();
  //Plays the whole game
  void play();

};

#endif