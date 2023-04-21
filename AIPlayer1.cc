#include "Player.hh"

#define PLAYER_NAME Player1

struct PLAYER_NAME : public Player{
  static Player* factory(){
    return new PLAYER_NAME;
  }

  //Global structures

  virtual void play(){

    //changing player

    //Collect units    
    vector<int> u = units(me());
    cerr << "u.size() = " << u.size() << endl;
    cerr << "player " << me() << ", printing units:" << endl;
    for(int i = 0; i < u.size(); ++i){
      cerr << u[i] << " ";
    }
    cerr << endl;

    if(round() == 0){ //first round, few local options
      move(u[0],Direction::up);
    }
    else if(round() == 1){  //same here
      move(u[0],Direction::UL);
    }

    
  }
};

REGISTER_PLAYER(PLAYER_NAME);