#ifndef Utility_hh
#define Utility_hh

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include "Timer.hh"

//using namespace std;

/**
 * Defines all directions
*/
enum Direction{null,up,down,left,right,UL,UR,DL,DR};
inline std::ostream& operator << (std::ostream& out, Direction d) {
  switch (d) {
  case Direction::down:   out << "Down";  break;
  case Direction::DR:     out << "DR";    break;
  case Direction::right:  out << "Right"; break;
  case Direction::UR:     out << "UR";    break;
  case Direction::up:     out << "Up";    break;
  case Direction::UL:     out << "UL";    break;
  case Direction::left:   out << "Left";  break;
  case Direction::DL:     out << "DL";    break;
  default:     out << int(d);  break;
  }
  return out;
}

struct Position{
  int8_t x;
  int8_t y;
  Position();
  Position(int posx, int posy);
  bool operator== (const Position& p) const;
  bool operator!= (const Position& p) const;
  bool operator<(const Position& p) const;
  Position operator+(const Direction& d);
  Position& operator+= (const Direction& d);
  Position operator+(const Position& other);
  Position& operator+= (const Position& other);
  Position operator-(const Position& other);
  Position& operator-= (const Position& other);
  /* Pre: p is adjacent (diagonal counts) to this
     Returns the direction from this to p. Returns null if an unexpected situation occurs  */
  Direction to(Position p);
  friend std::ostream& operator<<(std::ostream& os, const Position& p);
};

inline std::ostream& operator<< (std::ostream& os, const Position& p) {
  return os << "(" << int(p.x) << ", " << int(p.y) << ")";
}

enum UnitType{ unit, bubble ,bonus };

struct Unit{
public:

  int id() const;
  int player() const;
  Position position() const;
  bool upgraded() const;
  int energy() const;
  UnitType type() const;
  Unit();
  Unit(int id);
  Unit(int id, int player, const Position& position, bool upgraded = false, int energy = 0);

private:
  friend class Board;
  friend class GameInfo;

  int id_;
  int pl;
  Position p;
  UnitType t;

  //unit params
  bool upg;
  int energ;
};

struct Bubble{
public:
  Bubble();
  Bubble(int id);
  Bubble(int id, int player, Position pos, int rtp);
  int roundsToPop() const;
  int player() const;
  UnitType type() const;
  Position position()const;
private:
  friend class Board;
  friend class GameInfo;

  Position p;
  UnitType t;
  short id_;
  int8_t pl;

  //bubble params
  int8_t rtp;
};

struct Bonus{
public:
  Bonus();
  Bonus(Position pos);
  Bonus(int bid, Position pos);
private:
  friend class Board;
  friend class GameInfo;

  Position p;  //(-1,-1) means it is not spawned
  UnitType type;
  short id;
  
};

enum OrderType{ movement, attack, ability };

struct Order{
  int unitId;
  Direction dir;
  OrderType type;
  Order(){}
  Order(int id,Direction d, OrderType t){unitId = id; dir = d; type = t;}
};

struct Square{
public:

  //Square's position
  Position pos() const;
  //True if square is painted by some player, false otherwise
  bool painted() const;
  //Pre: Square is painted
  //Returns the id of the painter player
  int painter() const;
  //True if square is drawn by some player, false otherwise
  bool drawn() const;
  //Pre: Square is drawn
  //Returns the id of the drawer player
  int drawer() const;
  //Pre: Square is drawn
  //Returns the id of the drawer unit
  int unitDrawer() const;
  //Returns true if this Square separates territories
  bool border() const;
  //Returns true if this Square is currently from an ability
  bool ability() const;
  //Returns the number of rounds left for the ability
  int roundsUntilAbilityEnd() const;

  //Returns true if the Square is empty, false otherwise
  bool empty() const;
  //Returns true if the Square has specifically a Unit, false otherwise
  bool hasUnit() const;
  //Pre: Square has a Unit
  //Returns such Unit
  Unit unit() const;
  //Returns true if the Square has specifically a Bonus, false otherwise
  bool hasBonus() const;
  //Pre: Square has a Bonus
  //Returns such bonus
  Bonus bonus() const;
  //Returns true if the Square has specifically a Bubble, false otherwise
  bool hasBubble() const;
  //Pre: Square has a Bubble
  //Returns such Bubble
  Bubble bubble() const;
  
  //Default constructor
  Square();
  //Copy constructor
  Square(const Square& sq);

private:

  friend class Board;
  friend class GameInfo;
  Unit* u;        //nullptr if there is not a unit
  Bonus* b;       //nullptr if there is not a bonus
  Bubble* bb;     //nullptr if there is not a bubble
  Position p;
  int8_t plPainter;  //Player
  int8_t plDrawer;   //Player
  int8_t uDrawer;    //Unit id
  bool isBorder;
  bool closes;    //Used only for painting
  bool isAbility;
  int8_t counter;   //Tells rounds before wearing off. -1 means no counter
};

template<typename T>
//Matrix indexed by Position
class Matrix{
public:
  Matrix();
  Matrix(int rows, int cols);
  //Copy constructor
  Matrix(const Matrix& other);
  Matrix& operator=(const Matrix& other);
  //Move constructor
  ~Matrix();
  inline T& operator[](Position);
  inline const T& operator[](Position)const;
  inline int rows()const;
  inline int cols()const;

private:
  int r,c;
  T* _ptr = nullptr;
  //std::vector<T> data;
  size_t size;
};

//Matrix
template<typename T>
Matrix<T>::Matrix(){
  r = 0; c = 0;
  size = 0;
  _ptr = nullptr;
}
template<typename T>
Matrix<T>::Matrix(int rows, int cols){
  r = rows;   c = cols;
  size = r*c;
  _ptr = new T[size];
}
template<typename T>
Matrix<T>::Matrix(const Matrix<T>& other){
  r = other.r;
  c = other.c;
  size = other.size;
  if(_ptr != nullptr){
    delete[] _ptr;
  }
  _ptr = new T[size];
  for(int i = 0; i < size; ++i){
    _ptr[i] = other._ptr[i];
  }
}
template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other){
  r = other.r;
  c = other.c;
  size = other.size;
  if(_ptr != nullptr){
    delete[] _ptr;
  }
  _ptr = new T[size];
  for(int i = 0; i < size; ++i){
    _ptr[i] = other._ptr[i];
  }
  return *this;
}
template<typename T>
Matrix<T>::~Matrix(){
  delete[] _ptr;
}
template<typename T>
T& Matrix<T>::operator[](Position index){
  if(index.x < 0 or index.x >= r) std::__throw_out_of_range("ERROR: tried to access Matrix with incorrect row");
  if(index.y < 0 or index.y >= c) std::__throw_out_of_range("ERROR: tried to access Matrix with incorrect column");
  return _ptr[index.x*c+index.y];
}
template<typename T>
const T& Matrix<T>::operator[](Position index)const{
  if(index.x < 0 or index.x >= r) std::__throw_out_of_range("ERROR: tried to access Matrix with incorrect row");
  if(index.y < 0 or index.y >= c) std::__throw_out_of_range("ERROR: tried to access Matrix with incorrect column");
  return _ptr[index.x*c+index.y];
}
template<typename T>
int Matrix<T>::rows()const{return r;}
template<typename T>
int Matrix<T>::cols()const{return c;}
#endif