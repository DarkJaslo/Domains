#ifndef Utility_hh
#define Utility_hh

#include <iostream>
#include <vector>
#include <queue>
#include <set>

typedef unsigned char int8;
using namespace std;

/**
 * Defines all directions
*/
enum Direction{ up,down,left,right,UL,UR,DL,DR,null };

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

enum UnitType{ unit, bubble,bonus };

struct Unit{
public:

  int id() const;
  int player() const;
  Position position() const;
  bool upgraded() const;
  int energy() const;
  int roundsToPop() const;
  UnitType type() const;
  Unit();
  Unit(int id);
  Unit(int id, int player, const Position& position, bool upgraded = false, int energy = 0, UnitType type = UnitType::unit, int roundsToPop = -1);

private:
  friend class Board;

  int id_;
  int pl;
  Position p;
  UnitType t;

  //unit params
  bool upg;
  int energ;

  //bubble params
  int rtp;  //rounds to pop
};

struct Bonus{
  int id;
  Position p;
  UnitType type;
};

enum OrderType{ movement, attack, ability };

struct Order{
  int unitId;
  Direction dir;
  OrderType type;
  Order(){}
  Order(int id,Direction d, OrderType t){unitId = id; dir = d; type = t;}
};

struct Square{
public:

  Position pos() const;
  bool painted() const;
  bool drawed() const;
  bool border() const;
  int painter() const;
  //Returns an uid
  int drawer() const;
  bool hasUnit() const;
  Unit unit() const;
  bool hasBonus() const;
  Bonus bonus() const;
  Square();

private:
  friend class Board;
  Position p;
  bool isBorder;
  bool closes;
  int plPainter;  //Player
  int plDrawer;   //Player
  int uDrawer;
  Unit* u;        //nullptr if there is not an unit
  Bonus* b;
};



#endif