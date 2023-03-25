#include "Register.hh"

typedef std::map<std::string,Player*> factoryMap;

factoryMap* reg_ = 0;

bool Register::RegisterPlayer(const char* name, Player* p){
  if(reg_ = nullptr) reg_ = new factoryMap();
  if((*reg_).find(name) != (*reg_).end()) return false;
  (*reg_)[name] = p;
  return true;
}

Player* Register::newPlayer(std::string name){
  factoryMap::const_iterator it = reg_->find(name);
  if(it == reg_->end()){
    std::cerr << "Player " << name << " not registered" << std::endl;
    exit(1);
  }
  return it->second;
}