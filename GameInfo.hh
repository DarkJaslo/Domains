#ifndef GameInfo_hh
#define GameInfo_hh

#include "Utility.hh"

class GameInfo{
public:

  int cols() const;
  int rows() const;
  Square square(const Position& p) const;
  int painter(const Position& p) const;
  bool posOk(const Position& p)const;
  void printSettings();
  static int randomNumber(int l, int r); //implemented in Board.cc
  static vector<int> randomPermutation(); //implemented in Board.cc
  vector<int> units(int player) const;

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
  vector<int> whoHasWhat;  //if v[0] = 1, player 1 has unit with id 0
  vector<Unit> unitsVector;
  vector<int> points;
  vector<int> bonusPlayers;
};

#endif