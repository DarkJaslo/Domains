#ifndef PlayerOrders_hh
#define PlayerOrders_hh
#include "Utility.hh"

class PlayerOrders{
public:
  void move(int unitId, Direction dir);
  void attack(int unitId, Direction dir);
  void ability(int unitId);
  PlayerOrders();
  
private:
  friend class Game;
  friend class Board;
  void printOrderIds();
  void resetList();
  int maxSize;
  int index; //orderList[0,index] has valid orders. If index == -1, no orders
  vector<Order> orderList;
};

#endif