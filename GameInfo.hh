#ifndef GameInfo_hh
#define GameInfo_hh

#include "Utility.hh"

class GameInfo{
public:

  //Returns the number of columns the board has
  static int cols();
  //returns the number of rows the board has
  static int rows();
  //returns the current round
  static int round();
  //returns the points of player pl
  static int points(int pl);
  //Returns the square located in position p
  static Square square(const Position& p);
  //Returns the unit with id uid
  static Unit unit(int uid);
  //Returns which player owns this position, -1 if it is blank
  static int painter(const Position& p);
  //Returns which player is drawing here, -1 if it is blank
  static int drawerPlayer(const Position& p);
  //True if p is within the board, false if it is not
  static bool posOk(const Position& p);
  //Prints all game settings
  static void printSettings();
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
  static void readSettings();
  //Returns true and leaves the position of a free square owned by p. False if there is no free position
  static bool freeSquare(int plId, Position& p);
  //Spawns a unit
  static void spawnUnit(int plId, Position p);
  //Spawns a bubble
  static void spawnBubble(int plId, Position p);
  static int numPlayers;
  static int unitsStart;
  static int unitsMax;
  static int unitsMin;
  static int squaresMax;
  static int bonusMax;
  static int bubblesMax;
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
  static int abilityDuration;
  static int boardWidth;
  static int boardHeight;
  static int roundsToPop;
  static int currentRound;
  static vector<vector<char>> game_map;
  static vector<vector<Square>> square_map;
  static vector<vector<Square>> old_square_map;
  static vector<int> whoHasWhat;  //if v[0] = 1, player 1 has unit with id 0
  static vector<Unit> unitsVector;
  static vector<Bubble> bubblesVector;
  static int currentBubbles;
  static vector<int> playerPoints;
  static vector<int> bonusPlayers;
  static vector<Bonus> bonusVector;
  static int currentBonuses;

  //Spawn events management
  static vector<vector<Position>> player_squares;
  static vector<int> respawnCounters;
  static vector<int> bubbleCounters;
  static vector<int> bonusCounters;
};

#endif