#include "Utility.hh"

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
  case Direction::left:{ p.y--; break;}
  case Direction::right:{ p.y++; break;}
  case Direction::up:{ p.x--; break;}
  case Direction::down:{ p.x++; break;}
  case Direction::UL:{ p.x--; p.y--; break;}
  case Direction::UR:{ p.x--; p.y++; break;}
  case Direction::DL:{ p.x++; p.y--; break;}
  case Direction::DR:{ p.x++; p.y++; break;}
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