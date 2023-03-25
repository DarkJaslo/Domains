#ifndef Order_hh
#define Order_hh
#include "Utility.hh"

class Order{
  struct OrderData{
    int unitId;
    Direction dir;
    OrderType type;
  };
public:
  void move(int unitId, Direction dir);
  void attack(int unitId, Direction dir);
  void ability(int unitId);
  
private:
  friend class Game;
  int maxSize;
  int index;
  void executeOrder();
  vector<Order> orderList;
};

#endif