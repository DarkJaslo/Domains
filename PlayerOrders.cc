#include "PlayerOrders.hh"

void PlayerOrders::move(int unitId, Direction dir){
  Order o(unitId,dir,OrderType::movement);
  orderList.push_back(o);
}
void PlayerOrders::attack(int unitId, Direction dir){
  Order o(unitId,dir,OrderType::attack);
  orderList.push_back(o);
}
void PlayerOrders::ability(int unitId){
  Order o(unitId,Direction::null,OrderType::ability);
  orderList.push_back(o);
}

void PlayerOrders::printOrderIds(){
  for(const Order& o : orderList){
    cerr << o.unitId << endl;
  }
}