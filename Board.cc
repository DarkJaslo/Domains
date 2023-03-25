#include "Board.hh"

//Board
Board::Board(){}

void Board::iniBoard(){
  info.readSettings();
  info.game_map = vector<vector<char>>(info.boardHeight,vector<char>(info.boardWidth,'.'));
  info.square_map = vector<vector<Square>>(info.boardHeight,vector<Square>(info.boardWidth));
  for(int i = 0; i < info.boardHeight; ++i){
    for(int j = 0; j < info.boardWidth; ++j)
      info.square_map[i][j].p = Position(i,j);
  }
}

void Board::executeRound(const vector<Player*>& pl){
}

void Board::printRound(){
  for(int i = 0; i < info.boardHeight; ++i){
    for(int j = 0; j < info.boardWidth; ++j){
      cout << info.game_map[i][j];
      if(j < info.boardWidth-1) cout << " ";
    }
    cout << endl;
  }
  cout << endl;
}

void Board::printSettings(){
  info.printSettings();
}
