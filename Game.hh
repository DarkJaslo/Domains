#ifndef Game_hh
#define Game_hh

#include <iostream>
#include <vector>
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

class Player{
  int id;
  virtual void play();
};

struct Unit{
  int id;
  Player* player;
};

struct Square{
  Position p;
  bool painted;
  bool drawed;
  Player* painter;
  Player* drawer;
  Square();
};

class Board{
  public:
  Board();
  Board(int h, int w);
  int cols();
  int rows();
  Square square(const Position& p);
  private:
  vector<vector<int8>> game_map;
  vector<vector<Square>> square_map;
};

class Game{



};


}



#endif