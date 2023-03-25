#ifndef Utility_hh
#define Utility_hh

#include <iostream>
#include <vector>
#include <queue>

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

enum OrderType{ movement, attack, ability };

#endif