#ifndef Game_hh
#define Game_hh

#include <iostream>
#include <vector>
#include <queue>
#include "Player.hh"
using namespace std;

namespace game{

typedef unsigned char int8;

/**
 * Defines all directions
*/
enum Direction{ up,down,left,right,UL,UR,DL,DR };

struct Position{
  int x;
  int y;
  Position();
  Position(int posx, int posy);
  bool operator== (const Position& p);
  bool operator!= (const Position& p);
  bool operator<(const Position& p);
  Position operator+(const Direction& d);
  Position& operator+= (const Direction& d);
};

enum OrderType{ movement, attack };
class Order{
  struct OrderData{
    int unitId;
    Direction dir;
    OrderType type;
  };
public:
  void move(int unitId, Direction dir);
  void attack(int unitId, Direction dir);
  void ability(int unitId);
  
private:
  friend class Game;
  int maxSize;
  int index;
  void executeOrder();
  vector<Order> orderList;
};

enum UnitType{ unit, bubble,bonus };

struct Unit{
public:

  int id() const;
  int player() const;
  Position position() const;
  bool upgraded() const;
  int energy() const;
  int roundsToPop() const;

private:

  int id_;
  int pl;
  Position p;
  UnitType type;

  //unit params
  bool upg;
  int energ;

  //bubble params
  int rtp;  //rounds to pop

  Unit();
};

struct Bonus{
  int id;
  Position p;
  UnitType type;
};

struct Square{
public:

  Position pos() const;
  bool painted() const;
  bool drawed() const;
  int painter() const;
  int drawer() const;
  Square();

private:
  friend class Board;
  Position p;
  bool isPainted;
  bool isDrawed;
  int plPainter;  //Player
  int plDrawer;   //Player
};

class Board{
  public:
  Board();
  Board(int h, int w);
  int cols() const;
  int rows() const;
  Square square(const Position& p) const;
  private:
  vector<vector<int8>> game_map;
  vector<vector<Square>> square_map;
};

class Game{

public:
  int board_cols() const;
  int board_rows() const;

private:
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
  int energyMax;
  int energyMin;
  int abilitySize;
  vector<Player*> players;
  vector<Order*> orders;
  vector<int> roundsSinceRespawn;

  Game();
  //Plays the whole game
  void play();

};


}
#endif