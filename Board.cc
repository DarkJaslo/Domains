#include "Board.hh"

Board::Board(){}
Board::Board(int height, int width){
  game_map = vector<vector<int>>(height,vector<int>(width,0));
}
int Board::cols(){ return game_map[0].size(); }
int Board::rows(){ return game_map.size(); }