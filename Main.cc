#include <iostream>
#include "Game.hh"
#include "Timer.hh"
#include <unistd.h>
using namespace std;

int main(int argc, char** argv){

  cout.setf(ios::fixed);
  cout.precision(3);

  //float t1,t2;

  //t1 = GetTime();
  Timer timer("main",nullptr);

  //1:seed 2:pl1 3:pl2 4:pl3 5:pl4

  cerr << "hola soy el main" << endl;
  //Register::printPlayers();

  int nplayers = argc-3;
  if(nplayers < 2){
    cerr << "too few players/parameters" << endl;
    exit(1);
  }
  else if(nplayers > 4){
    cerr << "too many players/parameters" << endl;
    exit(1);
  }

  Game g;
  vector<string> names(nplayers);
  for(int i = 3; i-3 < names.size(); ++i){
    names[i-3] = argv[i];
  }

  string arg2 = argv[2];
  bool show = arg2 == "y";

  g.play(names,atoi(argv[1]),false,show /*,true*/);

  cerr << "Fin main" << endl;

  //t2 = GetTime();
  //cerr << "Tiempo: " << t2-t1 << "ms" << endl;
  //exit(0);
}