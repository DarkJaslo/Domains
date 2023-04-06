#include <iostream>
#include "Game.hh"
using namespace std;

int main(int argc, char** argv){
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
  exit(0);
}