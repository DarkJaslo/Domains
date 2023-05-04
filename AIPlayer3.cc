#include "Player.hh"

#define PLAYER_NAME Player3

struct PLAYER_NAME : public Player{
  static Player* factory(){
    return new PLAYER_NAME;
  }

  virtual void play(){
    //move(2,Direction::null);
  }
};

REGISTER_PLAYER(PLAYER_NAME);