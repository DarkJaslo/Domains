#ifndef Register_hh
#define Register_hh
#include <string>
#include <map>
#include <iostream>
#define _stringification(s) #s
#define REGISTER_PLAYER(p) static bool registration = \
        Register::RegisterPlayer(_stringification(p), p::factory)

class Player; //Defines the Player type to use

class Register{

public:

  static bool RegisterPlayer(const char* name, Player* p);
  static Player* newPlayer(std::string name);
};

#endif