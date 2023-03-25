#ifndef PlayerOrders_hh
#define PlayerOrders_hh
#include "Utility.hh"

class PlayerOrders{
  struct Order{
    int unitId;
    Direction dir;
    OrderType type;
    Order(int id,Direction d, OrderType t){unitId = id; dir = d; t = type;}
  };
public:
  void move(int unitId, Direction dir);
  void attack(int unitId, Direction dir);
  void ability(int unitId);
  
private:
  void printOrderIds();
  friend class Game;
  int maxSize;
  vector<Order> orderList;
};

#endif