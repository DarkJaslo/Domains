#include "Game.hh"
using namespace game;

//Position
Position::Position(){ x = 0; y = 0;};
Position::Position(int posx, int posy){ x = posx; y = posy; }
bool Position::operator== (const Position& p){return x == p.x and y == p.y;}
bool Position::operator!= (const Position& p){return x != p.x or y != p.y;}
bool Position::operator<(const Position& p){
  if(x+y == p.x+p.y) return x < p.y;
  return x+y < p.x+p.y; 
}
Position Position::operator+ (const Direction& d){
  Position p;
  switch (d){
  case left:{ p.y--; break;}
  case right:{ p.y++; break;}
  case up:{ p.x--; break;}
  case down:{ p.x++; break;}
  case UL:{ p.x--; p.y--; break;}
  case UR:{ p.x--; p.y++; break;}
  case DL:{ p.x++; p.y--; break;}
  case DR:{ p.x++; p.y++; break;}
  default:{break;}
  }
  return p;
}
Position& Position::operator+= (const Direction& d){
  *this = *this+d;
  return *this;
}

//Square
Square::Square(){p=Position(-1,-1); isPainted=false; isDrawed=false; plPainter=-1; plDrawer=-1;}

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

//Game

Game::Game(){

}
void Game::play(){

}