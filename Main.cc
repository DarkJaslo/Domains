#include <iostream>
#include "Game.hh"
#include "time.cc"
#include <unistd.h>
using namespace std;

int main(int argc, char** argv){
  float t1,t2;

  t1 = GetTime();

  cerr << "hola soy el main" << endl;
  Register::printPlayers();
  if(argc < 3){
    cerr << "not enough players" << endl;
    exit(1);
  }
  if(argc < 4){
    cerr << "missing seed" << endl;
    exit(1);
  }
  Game g;
  vector<string> names(2);
  names[0] = argv[1];
  names[1] = argv[2];
  g.play(names,atoi(argv[3]),true);

  cerr << "Fin main" << endl;

  t2 = GetTime();
  cerr << "Tiempo: " << t2-t1 << "ms" << endl;
  exit(0);
}