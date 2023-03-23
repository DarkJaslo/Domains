#ifndef Board_hh
#define Board_hh

#include "Utils.hh"

class Board{
public:
  Board();
  Board(int height, int width);
  int cols();
  int rows();
private:
  vector<vector<int>> game_map; 
};

#endif