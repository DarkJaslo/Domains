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
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 5){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 6){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 7){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 8){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 9){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 10){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 11){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 12){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 13){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 14){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 15){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 16){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 17){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 18){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 19){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 20){
      cerr << "round " << round() << endl;
      move(u[0],Direction::null);
      cerr << "moved" << endl;
    }
    if(round() == 21){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 22){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 23){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 24){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 25){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 26){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 27){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 28){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 29){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 30){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 31){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 32){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 33){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 34){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 35){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 36){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 36){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 38){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 39){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 40){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 41){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 42){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 43){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 44){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    if(round() == 45){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 46){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    if(round() == 47){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }


    /*
    pattern 1
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
    if(round() == 18){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    if(round() == 19){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    if(round() == 17){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }*/
    //move(1,Direction::right);
  }
};

REGISTER_PLAYER(PLAYER_NAME);