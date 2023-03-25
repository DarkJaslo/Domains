#include "Player.hh"

#define PLAYER_NAME Player1

struct PLAYER_NAME : public Player{
  static Player* factory(){
    return new PLAYER_NAME;
  }

  virtual void play(){}
};

REGISTER_PLAYER(PLAYER_NAME);