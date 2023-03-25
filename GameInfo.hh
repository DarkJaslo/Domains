#ifndef GameInfo_hh
#define GameInfo_hh

#include "Utility.hh"

class GameInfo{
public:

  int cols() const;
  int rows() const;
  Square square(const Position& p) const;
  void printSettings();

private:
  friend class Board;

  void readSettings();
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
  int energyStart;
  int energyMax;
  int energyMin;
  int abilitySize;
  int boardWidth;
  int boardHeight;
  vector<vector<char>> game_map;
  vector<vector<Square>> square_map;
  vector<int> roundsSinceRespawn;
};

#endif