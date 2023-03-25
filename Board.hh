#include "Utility.hh"

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