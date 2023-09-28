#include <iostream>
#include "Game.hh"
#include "Timer.hh"
#include <unistd.h>
using namespace std;

int main(int argc, char** argv){

  cout.setf(ios::fixed);
  cout.precision(3);
  
  Timer timer("main",nullptr);

  //1:seed 2:print? 3:pl1 4:pl2 5:pl3 6:pl4

  if(argc != 6)
  {
    std::cerr << "Usage: ./Game.exe [seed] [print_options] [Player1] [Player2] [Player3] [Player4] < [config_file]" << std::endl;
  }

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
  std::vector<string> names(nplayers);
  for(int i = 3; i-3 < names.size(); ++i){
    names[i-3] = argv[i];
  }

  string arg2 = argv[2];
  bool show = arg2 == "y";

  g.play(names,atoi(argv[1]),true,show /*,true*/);

  cerr << "Fin main" << endl;
  //exit(0);
}