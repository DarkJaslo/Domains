#include "PlayerOrders.hh"

void PlayerOrders::move(int unitId, Direction dir){
  Order o(unitId,dir,OrderType::movement);
  ++index;
  if(index == orderList.size()){
    orderList.push_back(o);
  }
  else{
    orderList[index] = o;
  }
}
void PlayerOrders::attack(int unitId, Direction dir){
  Order o(unitId,dir,OrderType::attack);
  ++index;
  if(index == orderList.size()){
    orderList.push_back(o);
  }
  else{
    orderList[index] = o;
  }
}
void PlayerOrders::ability(int unitId){
  Order o(unitId,Direction::null,OrderType::ability);
  ++index;
  if(index == orderList.size()){
    orderList.push_back(o);
  }
  else{
    orderList[index] = o;
  }
}

PlayerOrders::PlayerOrders(){
  index = -1;
}

void PlayerOrders::printOrderIds(){
  for(const Order& o : orderList){
    cerr << o.unitId << endl;
  }
}

void PlayerOrders::resetList(){
  index = -1;
}