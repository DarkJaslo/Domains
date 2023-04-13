#include "Utility.hh"

//Position
Position::Position(){ x = 0; y = 0;};
Position::Position(int posx, int posy){ x = posx; y = posy; }
bool Position::operator== (const Position& p){return x == p.x and y == p.y;}
bool Position::operator!= (const Position& p){return x != p.x or y != p.y;}
bool Position::operator<(const Position& p){
  if(x != p.x) return x < p.x;
  return y < p.y;
}
Position Position::operator+ (const Direction& d){
  Position p = *this;
  p += d;  
  return p;
}
Position& Position::operator+= (const Direction& d){
  switch (d){
  case Direction::left:{ y--; break;}
  case Direction::right:{ y++; break;}
  case Direction::up:{ x--; break;}
  case Direction::down:{ x++; break;}
  case Direction::UL:{ x--; y--; break;}
  case Direction::UR:{ x--; y++; break;}
  case Direction::DL:{ x++; y--; break;}
  case Direction::DR:{ x++; y++; break;}
  default:{break;}
  }
  return *this;
}


//Square
Square::Square(){p=Position(-1,-1); plPainter=-1; plDrawer=-1;}
Position Square::pos()const{return p;}
bool Square::painted()const{return plPainter != -1;}
bool Square::drawed()const{return uDrawer != -1;}
bool Square::border()const{return isBorder;}
int Square::painter()const{return plPainter;}
int Square::drawer()const{return uDrawer;}
bool Square::hasUnit()const{return u != nullptr;}
Unit Square::unit()const{return *u;}
bool Square::hasBonus()const{return b != nullptr;}
Bonus Square::bonus()const{return *b;}

//Unit

Unit::Unit(){
  pl = -1; upg = false; t = UnitType::unit; rtp = -1;
}
Unit::Unit(int id){
  Unit();
  id_ = id;
}
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