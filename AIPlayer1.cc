#include "Player.hh"

#define PLAYER_NAME Player1

struct PLAYER_NAME : public Player{
  static Player* factory(){
    return new PLAYER_NAME;
  }

  virtual void play(){
    
    vector<int> u = units(me());
    cerr << "u.size() = " << u.size() << endl;
    if(round() == 0){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 1){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 2){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 3){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 4){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 5){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 6){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 7){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 8){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 9){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 10){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 11){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 12){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 13){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 14){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 15){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 16){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 17){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    //move(1,Direction::right);
  }
};

REGISTER_PLAYER(PLAYER_NAME);