#include "Utility.hh"
#include "GameInfo.hh"
#include "Player.hh"

class Board{
public:
  Board();
  void iniBoard();
  void executeRound(const vector<Player*>& pl);
  void printRound();
  void printSettings();
private:
  GameInfo info;
  //vector<Player*> players;
};