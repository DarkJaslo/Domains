#ifndef GameInfo_hh
#define GameInfo_hh

#include "Utility.hh"

class GameInfo{
public:

  ~GameInfo();
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
  //Returns the unit with position p
  static Unit unit(Position p);
  //Returns the bubble with id bid
  static Bubble bubble(int bid);
  //Returns the bubble with position p
  static Bubble bubble(Position p);
  //True if p is within the board, false if it is not
  static bool posOk(const Position& p);
  //Prints all game settings
  static void printSettings();
  //Returns a number inside  [l,r]
  static int randomNumber(int l, int r); //implemented in Board.cc
  //Returns a random permutation of 0,1,2,3
  static std::vector<int> randomPermutation(); //implemented in Board.cc
  //Returns all unit ids the player p possesses
  static std::vector<int> units(int p);
  //Returns the number of units belonging to each player
  static std::vector<int> numberOfUnits();


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
  static int maxRounds;
  static int numPlayers;
  static int unitsStart;
  static int unitsMax;
  static int unitsMin;
  static int bonusMax;
  static int roundsPerRespawn;
  static int roundsPerBubble;
  static int roundsPerBubbleMove;
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
  static std::vector<std::vector<char>> game_map;
  static Matrix<Square> square_map;
  //static vector<vector<Square>> square_map;
  static Matrix<Square> old_square_map;
  //static vector<vector<Square>> old_square_map;
  static std::vector<int> whoHasWhat;  //if v[0] = 1, player 1 has unit with id 0
  static std::vector<Unit> unitsVector;
  static std::vector<Bubble> bubblesVector;
  static int currentBubbles;
  static std::vector<int> playerPoints;
  static std::vector<int> bonusPlayers;
  static std::vector<Bonus> bonusVector;
  static int currentBonuses;

  //Spawn events management
  static std::vector<std::vector<Position>> player_squares;
  static std::vector<int> respawnCounters;
  static std::vector<int> bubbleCounters;
  static std::vector<int> bubbleMovementCounters;
  static std::vector<int> bonusCounters;

};

#endif