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
  Position p = *this;
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
Position Square::pos()const{return p;}
bool Square::painted()const{return isPainted;}
bool Square::drawed()const{return isDrawed;}
bool Square::border()const{return isBorder;}
bool Square::closes()const{return isClosing;}
int Square::painter()const{return plPainter;}
int Square::drawer()const{return uDrawer;}
bool Square::hasUnit()const{return u != nullptr;}
Unit Square::unit()const{return *u;}
bool Square::hasBonus()const{return b != nullptr;}
Bonus Square::bonus()const{return *b;}

//Unit

Unit::Unit(){}
Unit::Unit(int id, int player, const Position& position, bool upgraded, int energy, UnitType type, int roundsToPop){
  id_ = id; pl = player; p = position; upg = upgraded; energ = energy; t = type; rtp = roundsToPop;
}
int Unit::id()const{return id_;}
int Unit::energy()const{return energ;}
bool Unit::upgraded()const{return upg;}
int Unit::player()const{return pl;}
Position Unit::position()const{return p;}
UnitType Unit::type()const{return t;}
int Unit::roundsToPop()const{return rtp;}