#include <iostream>
#include "Game.hh"
#include "time.cc"
#include <unistd.h>
using namespace std;

int main(int argc, char** argv){
  float t1,t2;

  t1 = GetTime();

  //1:seed 2:pl1 3:pl2 4:pl3 5:pl4

  cerr << "hola soy el main" << endl;
  //Register::printPlayers();

  int nplayers = argc-2;
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
  for(int i = 2; i-2 < names.size(); ++i){
    names[i-2] = argv[i];
  }

  g.play(names,atoi(argv[1]),true);

  cerr << "Fin main" << endl;

  t2 = GetTime();
  cerr << "Tiempo: " << t2-t1 << "ms" << endl;
  exit(0);
}