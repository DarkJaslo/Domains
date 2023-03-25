#ifndef Register_hh
#define Register_hh
#include <string>
#include <map>
#include <iostream>

class Player; //Defines the Player type to use

class Register{

public:

  typedef Player* (*Factory)();
  static int RegisterPlayer(const char* name, Factory f);
  static Player* newPlayer(std::string name);
  static void printPlayers();
};

#define _stringification(s) #s
#define REGISTER_PLAYER(p) static int registration = \
        Register::RegisterPlayer(_stringification(p), p::factory)
        
#endif