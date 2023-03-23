/**
 * Definitions and inclusions
*/

#include <iostream>
#include <vector>
using namespace std;

struct Position{
  int x;
  int y;
  Position(){};
  Position(int posx, int posy){ x = posx; y = posy; }
  bool operator== (const Position& p){return x == p.x and y == p.y;}
  bool operator!= (const Position& p){return x != p.x or y != p.y;}
  bool operator<(const Position& p){
    if(x+y == p.x+p.y) return x < p.y;
    return x+y < p.x+p.y; 
  }
};

enum Direction{
  up,
  down,
  left,
  right
};


struct Order{

};