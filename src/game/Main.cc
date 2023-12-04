#include <iostream>
#include "Game.hh"
#include "Timer.hh"
#include <unistd.h>
using namespace std;

int main(int argc, char** argv){

  cout.setf(ios::fixed);
  cout.precision(3);
  
  Timer timer("main",nullptr);

  //1:seed 2:print? 3:debug|run 4:pl1 5:pl2 6:pl3 7:pl4

  if(argc < 4 or argc > 8)
  {
    std::cerr << "Usage: ./Game.exe [seed] [print {y|n}] [parallel {debug|run}] [Player1] [Player2] [Player3] [Player4] < [config_file]\n\n";
    std::cerr << "Print allows choosing if the board is printed to stdout every round. Set to 'n' if you won't need the result file.\n";
    std::cerr << "Parallel allows running players' play() functions in parallel (faster). Set to debug if you want console prints to make sense.\n";
    std::cerr << "You can enter any number of players between 1 and 4.\n";
    exit(EXIT_FAILURE);
  }

  cerr << "Main\n";
  //Register::printPlayers();

  int nplayers = argc-4;

  Game g;
  std::vector<string> names(nplayers);
  for(int i = 4; i-4 < names.size(); ++i){
    names[i-4] = argv[i];
  }

  string arg2 = argv[2];
  bool show = arg2 == "y";

  string arg3 = argv[3];
  bool run;
  if(arg3 == "debug")
    run = false;
  else if(arg3 == "run")
    run = true;
  else
  {
    string err = "Bad argument value for [debug|run]: ";
    err.append(arg3);
    __throw_domain_error(err.c_str());
  }

  g.play(names,atoi(argv[1]),false,show,run /*,true*/);

  cerr << "Main end" << endl;
  //exit(0);
}