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
  int painter;  //Player
  int drawer;   //Player
  Square();
};

struct Unit{
  Position p;
  int energy;
  
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
  int numPlayers;
  int unitsStart;
  int unitsMax;
  int unitsMin;
  int squaresMax;
  int roundsPerRespawn;
  vector<Player*> players;
  vector<int> roundsSinceRespawn;

  //Plays the whole game
  void play();

};


}



#endif