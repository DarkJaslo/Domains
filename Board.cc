#include "Board.hh"

//Board
Board::Board(){}
Board::Board(int h,int w){
  game_map = vector<vector<int8>>(h,vector<int8>(w,0));
  square_map = vector<vector<Square>>(h,vector<Square>(w));
  for(int i = 0; i < h; ++i){
    for(int j = 0; j < w; ++j)
      square_map[i][j].p = Position(i,j);
  }
}
int Board::cols()const{return game_map[0].size();}
int Board::rows()const{return game_map.size();}
Square Board::square(const Position& p)const{
  if(p.y < cols() or p.y > cols() or p.x > rows() or p.x < rows()) cerr << "Position is not valid" << endl;
  return square_map[p.x][p.y];
}