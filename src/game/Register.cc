#include "Register.hh"

typedef std::map<std::string,Register::Factory> factoryMap;

factoryMap* reg_ = 0;

int Register::RegisterPlayer(const char* name, Factory f){
  //std::cerr << "call to RegisterPlayer" << std::endl;
  if(reg_ == 0) reg_ = new factoryMap();
  (*reg_)[name] = f;
  return 999;
}

Player* Register::newPlayer(std::string name){
  //std::cerr << "call to newPlayer" << std::endl;
  factoryMap::const_iterator it = reg_->find(name);
  if(it == reg_->end()){
    std::cerr << "Player " << name << " not registered" << std::endl;
    exit(1);
  }
  std::cerr << "creating player " << name << std::endl;
  return (it->second)();
}

void Register::printPlayers(){
  //std::cerr << "call to printPlayers" << std::endl;
  for(auto it = reg_->begin(); it != reg_->end(); ++it){
    std::cerr << it->first << std::endl;
  }
}