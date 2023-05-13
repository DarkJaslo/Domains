#ifndef Game_hh
#define Game_hh
#include "Player.hh"
#include "Utility.hh"
#include "Board.hh"
using namespace std;

class Game{

public:
  Game();
  //Plays the whole game
  void play(const vector<string>& vnames, int seed, bool fullDebug, bool view, bool debug = true);
  ~Game();

private:
  int rounds;
  Board board;
};

#endif