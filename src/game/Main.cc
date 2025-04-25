#include <iostream>
#include "Game.hh"
#include "Timer.hh"
#include <unistd.h>
#include <fstream>
using namespace std;

int main(int argc, char** argv){

  cout.setf(ios::fixed);
  cout.precision(3);
  
  Timer timer("main",nullptr);

  //1:seed 2:print? 3:debug|run 4:old_viewer|new_viewer | 5:pl1 6:pl2 7:pl3 8:pl4

  if(argc < 5 or argc > 10)
  {
    std::cerr << "Arg count: " << argc << std::endl;
    std::cerr << "Usage: ./Game.exe [seed] [print {y|n}] [parallel {debug|run}] [viewer {old|new}] [config_file] [Player1] [Player2] [Player3] [Player4]\n\n";
    std::cerr << "Print allows choosing if the board is printed to stdout every round. Set to 'n' if you won't need the result file.\n";
    std::cerr << "Parallel allows running players' play() functions in parallel (faster). Set to debug if you want console prints to make sense. [Parallel option is currently unavailable as it does not work as intended yet]\n";
    std::cerr << "The new viewer format creates much smaller game files (about 13x so)\n";
    std::cerr << "You can enter any number of players between 1 and 4.\n";
    exit(EXIT_FAILURE);
  }

  cerr << "Main\n";
  //Register::printPlayers();

  int nplayers = argc-6;

  Game g;
  std::vector<string> names(nplayers);
  for(int i = 6; i-6 < names.size(); ++i){
    names[i-6] = argv[i];
  }

  string arg2 = argv[2];
  bool show = arg2 == "y";

  string arg3 = argv[3];
  bool run;
  if(arg3 == "debug")
    run = false;
  else if(arg3 == "run")
    //run = true;
    run = false; //temporary, parallel does not work well
  else
  {
    string err = "Bad argument value for [debug|run]: ";
    err.append(arg3);
    __throw_domain_error(err.c_str());
  }

  string arg4 = argv[4];
  bool new_viewer;
  if (arg4 == "new")
    new_viewer = true;
  else if (arg4 == "old")
    new_viewer = false;
  else
  {
    string err = "Bad argument value for [new|old]: ";
    err.append(arg4);
    __throw_domain_error(err.c_str());
  }

  string arg5 = argv[5];
  std::ifstream config_file;
  config_file.open(arg5);
  if (!config_file.is_open())
  {
    std::cerr << "Could not open configuration file " << arg5 << std::endl;
    exit(EXIT_FAILURE);
  }

  g.play(names,
         atoi(argv[1]),
         false,
         show,
         run,
         /*debug*/ true,
         new_viewer,
        config_file);

  cerr << "Main end" << endl;
  //exit(0);
}