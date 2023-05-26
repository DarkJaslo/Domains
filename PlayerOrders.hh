#ifndef PlayerOrders_hh
#define PlayerOrders_hh
#include "Utility.hh"

class PlayerOrders{
public:
  PlayerOrders();

protected:
  void move(int unitId, Direction dir);
  void attack(int unitId, Direction dir);
  void ability(int unitId);
  
private:
  friend class Game;
  friend class Board;
  void printOrderIds();
  void resetList();
  int maxSize;
  int index; //orderList[0,index] has valid orders. If index == -1, no orders
  std::vector<Order> orderList;
  std::set<int> orderedUnits;
};

#endif