#include <iostream>
#include "Game.hh"
using namespace std;

int main(int argc, char** argv){
  cout << "hola soy el main" << endl;
  Register::printPlayers();
  if(argc != 3){
    cerr << "faltan/sobran nombres de jugadores" << endl;
    exit(1);
  }
  Game g;
  vector<string> names(2);
  names[0] = argv[1];
  names[1] = argv[2];
  g.play(names,true);

  cerr << "Fin main" << endl;
  exit(0);
}