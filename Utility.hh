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
enum Direction{null,up,down,left,right,UL,UR,DL,DR};

struct Position{
  int x;
  int y;
  Position();
  Position(int posx, int posy);
  bool operator== (const Position& p) const;
  bool operator!= (const Position& p) const;
  bool operator<(const Position& p) const;
  Position operator+(const Direction& d);
  Position& operator+= (const Direction& d);
  /* Pre: p is adjacent (diagonal counts) to this
     Returns the direction from this to p. Returns null if an unexpected situation occurs  */
  Direction to(Position p);
};

enum UnitType{ unit, bubble,bonus };

struct Unit{
public:

  int id() const;
  int player() const;
  Position position() const;
  bool upgraded() const;
  int energy() const;
  UnitType type() const;
  Unit();
  Unit(int id);
  Unit(int id, int player, const Position& position, bool upgraded = false, int energy = 0);

private:
  friend class Board;
  friend class GameInfo;

  int id_;
  int pl;
  Position p;
  UnitType t;

  //unit params
  bool upg;
  int energ;
};

struct Bubble{
public:
  Bubble();
  Bubble(int id);
  Bubble(int id, int player, Position pos, int rtp);
  int roundsToPop() const;
  int player() const;
  UnitType type() const;
  Position position()const;
private:
  friend class Board;
  friend class GameInfo;

  int id_;
  int pl;
  Position p;
  UnitType t;

  //bubble params
  int rtp;
};

struct Bonus{
public:
  Bonus();
  Bonus(Position pos);
private:
  friend class Board;
  friend class GameInfo;
  static int bonusId();
  static int bonusid;
  int id;
  Position p;  //(-1,-1) means it is not spawned
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
  bool empty() const;
  bool hasUnit() const;
  Unit unit() const;
  bool hasBonus() const;
  Bonus bonus() const;
  bool hasBubble() const;
  Bubble bubble() const;
  Square();

private:
  friend class Board;
  friend class GameInfo;
  Position p;
  bool isBorder;
  bool closes;
  int plPainter;  //Player
  int plDrawer;   //Player
  int uDrawer;
  Unit* u;        //nullptr if there is not a unit
  Bonus* b;
  Bubble* bb;
};



#endif