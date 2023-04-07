#include "PlayerOrders.hh"

void PlayerOrders::move(int unitId, Direction dir){
  cerr << "ordering move to unit " << unitId << endl;
  if(orderedUnits.find(unitId) != orderedUnits.end()){
    cerr << "error: unit " << unitId << " has already been given an order" << endl;
    return;
  }
  else{orderedUnits.insert(unitId);}

  Order o(unitId,dir,OrderType::movement);
  ++index;
  if(index == orderList.size()){
    orderList.push_back(o);
  }
  else{
    orderList[index] = o;
  }
  cerr << "finished ordering unit " << unitId << endl;
}
void PlayerOrders::attack(int unitId, Direction dir){
  cerr << "ordering attack to unit " << unitId << endl;
  if(orderedUnits.find(unitId) != orderedUnits.end()){
    cerr << "error: unit " << unitId << " has already been given an order" << endl;
    return;
  }
  else{orderedUnits.insert(unitId);}

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
  cerr << "ordering ability to unit " << unitId << endl;
  if(orderedUnits.find(unitId) != orderedUnits.end()){
    cerr << "error: unit " << unitId << " has already been given an order" << endl;
    return;
  }
  else{orderedUnits.insert(unitId);}

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
  orderedUnits.clear();
}