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
    else if(round() == 1){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 2){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 3){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 4){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 5){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 6){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 7){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 8){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 9){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 10){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 11){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 12){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 13){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 14){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 15){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 16){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 17){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 18){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 19){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 20){
      cerr << "round " << round() << endl;
      move(u[0],Direction::null);
      cerr << "moved" << endl;
    }
    else if(round() == 21){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 22){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 23){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 24){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 25){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 26){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 27){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 28){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 29){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 30){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 31){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 32){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 33){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 34){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 35){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 36){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 37){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 38){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 39){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 40){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 41){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 42){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 43){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 44){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 45){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 46){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 47){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 48){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 49){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 50){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 51){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 52){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 53){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 54){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 55){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 56){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 57){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 58){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 59){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 60){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 61){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 62){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 63){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 64){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 65){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 66){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }


    /*
    pattern 1
    if(round() == 0){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 1){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 2){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 3){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 4){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 5){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 6){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 7){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 8){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 9){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 10){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 11){
      cerr << "round " << round() << endl;
      move(u[0],Direction::down);
      cerr << "moved" << endl;
    }
    else if(round() == 12){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 13){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 14){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 15){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 16){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 17){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }
    else if(round() == 18){
      cerr << "round " << round() << endl;
      move(u[0],Direction::right);
      cerr << "moved" << endl;
    }
    else if(round() == 19){
      cerr << "round " << round() << endl;
      move(u[0],Direction::left);
      cerr << "moved" << endl;
    }
    else if(round() == 17){
      cerr << "round " << round() << endl;
      move(u[0],Direction::up);
      cerr << "moved" << endl;
    }*/
    //move(1,Direction::right);
  }
};

REGISTER_PLAYER(PLAYER_NAME);