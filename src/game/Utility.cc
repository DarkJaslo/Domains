#include "Utility.hh"

#include <tuple>

namespace utils
{
  std::tuple<Direction, Direction> decompose(Direction dir)
  {
    switch (dir)
    {
    case Direction::UL:
      return std::make_tuple(Direction::left, Direction::up);
    case Direction::UR:
      return std::make_tuple(Direction::right, Direction::up);
    case Direction::DL:
      return std::make_tuple(Direction::left, Direction::down);
    case Direction::DR:
      return std::make_tuple(Direction::right, Direction::down);
    default:
      return std::make_tuple(Direction::null, Direction::null);
    }
  }

  Direction invert(Direction dir)
  {
    switch (dir)
    {
    case Direction::up:
      return Direction::down;
    case Direction::down:
      return Direction::up;
    case Direction::left:
      return Direction::right;
    case Direction::right:
      return Direction::left;
    case Direction::UL:
      return Direction::DR;
    case Direction::UR:
      return Direction::DL;
    case Direction::DL:
      return Direction::UR;
    case Direction::DR:
      return Direction::UL;
    default:
      return Direction::null;
    }
  }

  bool isDiagonal(Direction dir)
  {
    switch (dir)
    {
    case Direction::DL:    
      return true;
    case Direction::DR:
      return true;    
    case Direction::UR:
      return true;    
    case Direction::UL:
      return true;    
    default:
      return false;
    }
  }
};

//Position
Position::Position(){ x = 0; y = 0;};
Position::Position(int posx, int posy){ x = static_cast<int8_t>(posx); y = static_cast<int8_t>(posy); }
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
Position Position::operator+(const Position& other){
  return Position(x+other.x,y+other.y);
}
Position& Position::operator+= (const Position& other){
  x+=other.x;
  y+=other.y;
  return *this;
}
Position Position::operator-(const Position& other){
  return Position(x-other.x,y-other.y);
}
Position& Position::operator-= (const Position& other){
  x-=other.x;
  y-=other.y;
  return *this;
}
Direction Position::to(Position p){
  Position vec{ p - *this };
  if (vec.x == 0 && vec.y == 0)
    return Direction::null;

  if (vec.x == 0)
  {
    if (vec.y > 0)
      return Direction::right;

    return Direction::left;
  }
  else if (vec.y == 0)
  {
    if (vec.x > 0)
      return Direction::down;

    return Direction::up;
  }
  else
  {
    if (vec.x > 0 && vec.y > 0)
      return Direction::DR;
    else if (vec.x < 0 && vec.y > 0)
      return Direction::UR;
    else if (vec.x > 0 && vec.y < 0)
      return Direction::DL;
    else
      return Direction::UL;
  } 
}

//Square
Square::Square(){p=Position(-1,-1); plPainter=-1; plDrawer=-1; isAbility = false; counter = 0;}
Square::Square(const Square& sq){
  u = sq.u;
  b = sq.b;     
  bb = sq.bb;
  p = sq.p;
  plPainter = sq.plPainter;
  plDrawer = sq.plDrawer;
  uDrawer = sq.uDrawer;
  isBorder = sq.isBorder;
  closes = sq.closes;
  isAbility = sq.isAbility;
  counter = sq.counter;
}
Position Square::pos()const{return p;}
bool Square::painted()const{return plPainter != -1;}
bool Square::drawn()const{return uDrawer != -1;}
bool Square::border()const{return isBorder;}
bool Square::ability()const{return isAbility;}
int Square::roundsUntilAbilityEnd()const{return static_cast<int>(counter);}
int Square::painter()const{return static_cast<int>(plPainter);}
int Square::drawer()const{return static_cast<int>(plDrawer);}
int Square::unitDrawer() const{ return static_cast<int>(uDrawer);}
bool Square::empty()const{return u == nullptr and b == nullptr and bb == nullptr;}
bool Square::hasUnit()const{return u != nullptr;}
Unit Square::unit()const{return *u;}
bool Square::hasBonus()const{return b != nullptr;}
Bonus Square::bonus()const{return *b;}
bool Square::hasBubble()const{return bb != nullptr;}
Bubble Square::bubble()const{return *bb;}

bool Square::operator==(Square const& other) const
{
  return u == other.u && plPainter == other.plPainter && plDrawer == other.plDrawer && isAbility == other.isAbility && b == other.b && bb == other.bb;
}

bool Square::operator!=(Square const& other) const
{
  return !(operator==(other));
}

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
Bubble::Bubble(int id){Bubble(); id_ = static_cast<short>(id);}
Bubble::Bubble(int id, int player, Position pos, int roundsToPop){ id_ = static_cast<short>(id); pl = player; p = pos; rtp = roundsToPop;}
int Bubble::roundsToPop()const{return static_cast<int>(rtp);}
int Bubble::player()const{return static_cast<int>(pl);}
Position Bubble::position()const{return p;}
UnitType Bubble::type()const{return t;}

//Bonus
Bonus::Bonus(){id = 0; p = Position(-1,-1); type = UnitType::bonus;}
Bonus::Bonus(Position pos){Bonus(); p = pos;}
Bonus::Bonus(int bid, Position pos){id = bid; p = pos; type = UnitType::bonus;}