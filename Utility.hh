#ifndef Utility_hh
#define Utility_hh

#include <iostream>
#include <vector>
#include <queue>
#include <set>

typedef char int8;
using namespace std;

/**
 * Defines all directions
*/
enum Direction{null,up,down,left,right,UL,UR,DL,DR};

struct Position{
  int8 x;
  int8 y;
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

enum UnitType{ unit, bubble ,bonus };

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

  Position p;
  UnitType t;
  short id_;
  int8 pl;

  //bubble params
  int8 rtp;
};

struct Bonus{
public:
  Bonus();
  Bonus(Position pos);
  Bonus(int bid, Position pos);
private:
  friend class Board;
  friend class GameInfo;

  Position p;  //(-1,-1) means it is not spawned
  UnitType type;
  short id;
  
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

  //Square's position
  Position pos() const;
  //True if square is painted by some player, false otherwise
  bool painted() const;
  //Pre: Square is painted
  //Returns the id of the painter player
  int painter() const;
  //True if square is drawed by some player, false otherwise
  bool drawed() const;
  //Pre: Square is drawed
  //Returns the id of the drawer player
  int drawer() const;
  //Returns true if this Square separates territories
  bool border() const;
  //Returns true if this Square is currently from an ability
  bool ability() const;
  //Returns the number of rounds left for the ability
  int roundsUntilAbilityEnd() const;

  //Returns true if the Square is empty, false otherwise
  bool empty() const;
  //Returns true if the Square has specifically a Unit, false otherwise
  bool hasUnit() const;
  //Pre: Square has a Unit
  //Returns such Unit
  Unit unit() const;
  //Returns true if the Square has specifically a Bonus, false otherwise
  bool hasBonus() const;
  //Pre: Square has a Bonus
  //Returns such bonus
  Bonus bonus() const;
  //Returns true if the Square has specifically a Bubble, false otherwise
  bool hasBubble() const;
  //Pre: Square has a Bubble
  //Returns such Bubble
  Bubble bubble() const;
  
  //Default constructor
  Square();

private:

  friend class Board;
  friend class GameInfo;
  Unit* u;        //nullptr if there is not a unit
  Bonus* b;       //nullptr if there is not a bonus
  Bubble* bb;     //nullptr if there is not a bubble
  Position p;
  int8 plPainter;  //Player
  int8 plDrawer;   //Player
  int8 uDrawer;    //Unit id
  bool isBorder;
  bool closes;    //Used only for painting
  bool isAbility;
  int8 counter;   //Tells rounds before wearing off. -1 means no counter
};
#endif