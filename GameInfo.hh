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
  //Returns the number of units belonging to each player
  static vector<int> numberOfUnits();


private:
  friend class Board;
  friend class Game;

  //Reads and initializes info
  void readSettings();
  //Returns true and leaves the position of a free square owned by p. False if there is no free position
  bool freeSquare(int plId, Position& p);
  //Spawns a unit
  void spawnUnit(int plId, Position p);
  //Spawns a bubble
  void spawnBubble(int plId, Position p);
  static int numPlayers;
  static int unitsStart;
  static int unitsMax;
  static int unitsMin;
  static int squaresMax;
  static int bonusMax;
  static int roundsPerRespawn;
  static int roundsPerBubble;
  static int roundsPerBonus;
  static int pointsPerUnit;
  static int pointsPerBubble;
  static int pointsPerPop;
  static int pointsPerSquare;
  static int energyStart;
  static int energyMax;
  static int energyMin;
  static int abilitySize;
  static int boardWidth;
  static int boardHeight;
  static int roundsToPop;
  static int currentRound;
  vector<vector<char>> game_map;
  vector<vector<Square>> square_map;
  vector<vector<Square>> old_square_map;
  vector<int> whoHasWhat;  //if v[0] = 1, player 1 has unit with id 0
  static vector<Unit> unitsVector;
  vector<Bubble> bubblesVector;
  vector<int> points;
  vector<int> bonusPlayers;
  Bonus bonus;

  //Spawn events management
  vector<int> respawnCounters;
  vector<int> bubbleCounters;
  int bonusCounter;
  
  vector<vector<Position>> player_squares;
};

#endif