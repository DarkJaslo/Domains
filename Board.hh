#include "Utility.hh"
#include "GameInfo.hh"
#include "Player.hh"

class Board{
public:
  Board();
  void iniBoard(int s);
  void executeRound(const vector<Player*>& pl);
  void printRound();
  void printSettings();
private:
  int seed;
  GameInfo info;
  //vector<Player*> players;
};