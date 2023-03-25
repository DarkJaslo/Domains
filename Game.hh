#ifndef Game_hh
#define Game_hh
#include "Player.hh"
#include "Utility.hh"
#include "PlayerOrders.hh"
#include "Board.hh"
using namespace std;

class Game{

public:
  Game();
  //Plays the whole game
  void play(const vector<string>& vnames, bool showInfo);

private:
  int rounds;
  Board board;
};

#endif