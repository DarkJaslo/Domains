#include "Board.hh"

//Board
Board::Board(){}

void Board::iniBoard(){
  info.game_map = vector<vector<int8>>(info.boardHeight,vector<int8>(info.boardWidth,0));
  info.square_map = vector<vector<Square>>(info.boardHeight,vector<Square>(info.boardWidth));
  for(int i = 0; i < info.boardHeight; ++i){
    for(int j = 0; j < info.boardWidth; ++j)
      info.square_map[i][j].p = Position(i,j);
  }
}
