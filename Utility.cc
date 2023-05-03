#include "Utility.hh"

//Position
Position::Position(){ x = 0; y = 0;};
Position::Position(int posx, int posy){ x = posx; y = posy; }
bool Position::operator== (const Position& p) const{return x == p.x and y == p.y;}
bool Position::operator!= (const Position& p) const{return x != p.x or y != p.y;}
bool Position::operator<(const Position& p) const{
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
bool Square::empty()const{return u == nullptr and b == nullptr and bb == nullptr;}
bool Square::hasUnit()const{return u != nullptr;}
Unit Square::unit()const{return *u;}
bool Square::hasBonus()const{return b != nullptr;}
Bonus Square::bonus()const{return *b;}
bool Square::hasBubble()const{return bb != nullptr;}
Bubble Square::bubble()const{return *bb;}

//Unit

Unit::Unit(){
  pl = -1; upg = false; t = UnitType::unit;
}
Unit::Unit(int id){
  Unit();
  id_ = id;
}
Unit::Unit(int id, int player, const Position& position, bool upgraded, int energy){
  id_ = id; pl = player; p = position; upg = upgraded; energ = energy; t = UnitType::unit;
}
int Unit::id()const{return id_;}
int Unit::energy()const{return energ;}
bool Unit::upgraded()const{return upg;}
int Unit::player()const{return pl;}
Position Unit::position()const{return p;}
UnitType Unit::type()const{return t;}

//Bubble

Bubble::Bubble(){pl = -1; t = UnitType::bubble;}
Bubble::Bubble(int id){Bubble(); id_ = id;}
Bubble::Bubble(int id, int player, Position pos, int roundsToPop){ id_ = id; pl = player; p = pos; rtp = roundsToPop;}
int Bubble::roundsToPop()const{return rtp;}
int Bubble::player()const{return pl;}
Position Bubble::position()const{return p;}
UnitType Bubble::type()const{return t;}

//Bonus
Bonus::Bonus(){id = 0; p = Position(-1,-1); type = UnitType::bonus;}
Bonus::Bonus(Position pos){Bonus(); p = pos;}
int Bonus::bonusId(){return 99;}