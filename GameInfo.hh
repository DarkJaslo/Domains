#ifndef GameInfo_hh
#define GameInfo_hh

#include "Utility.hh"

class GameInfo{
public:

  //Returns the number of columns the board has
  int cols() const;
  //returns the number of rows the board has
  int rows() const;
  //returns the current round
  static int round();
  //Returns the square located in position p
  Square square(const Position& p) const;
  //Returns which player owns this position, -1 if it is blank
  int painter(const Position& p) const;
  //Returns which player is drawing here, -1 if it is blank
  int drawerPlayer(const Position& p) const;
  //True if p is within the board, false if it is not
  bool posOk(const Position& p)const;
  //Prints all game settings
  void printSettings();
  //Returns a number inside  [l,r]
  static int randomNumber(int l, int r); //implemented in Board.cc
  //Returns a random permutation of 0,1,2,3
  static vector<int> randomPermutation(); //implemented in Board.cc
  //Returns all unit ids the player p possesses
  static vector<int> units(int p);


private:
  friend class Board;
  friend class Game;

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
  static int currentRound;
  vector<vector<char>> game_map;
  vector<vector<Square>> square_map;
  vector<vector<Square>> old_square_map;
  vector<int> roundsSinceRespawn;
  vector<int> whoHasWhat;  //if v[0] = 1, player 1 has unit with id 0
  static vector<Unit> unitsVector;
  vector<int> points;
  vector<int> bonusPlayers;

  //Spawn events management
  vector<int> respawnCounters;
  vector<int> blobCounters;
  int bonusCounter;
  
  vector<set<Position>> player_squares;
};

#endif