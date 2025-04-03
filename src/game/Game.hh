#ifndef Game_hh
#define Game_hh
#include "Player.hh"
#include "Utility.hh"
#include "Board.hh"

class Game{

public:
  Game();
  //Plays the whole game
  void play(const std::vector<std::string>& vnames, int seed, bool fullDebug, bool view, bool parallel, bool debug, bool new_viewer);
  ~Game();

private:
  int rounds;
  Board board;
};

#endif