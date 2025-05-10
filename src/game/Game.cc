#include "Game.hh"
#include "ThreadPool.h"
#include <thread>
#include <unistd.h>

#include "Timer.hh"

int current_player{-1};
std::vector<bool> crashed{};

//Game

Game::Game(){}
Game::~Game(){
  std::cerr << "deleting game" << std::endl;
}
void Game::play(const std::vector<std::string>& names, int seed, bool fullDebug, bool view, bool parallel, bool debug, bool new_viewer, std::ifstream& config_file){

  double ttotal, tplay, tround;
  ttotal = tplay = tround = 0.0;
  Timer timerall("Global timer",&ttotal,false);
  
  std::cerr << "starting game with seed " << seed << std::endl;
  std::vector<Player*> pl;
  std::string s;
  int rounds;
  config_file >> s >> rounds;

  std::cerr << "For real" << std::endl;

  Board b(fullDebug,view,names.size());
  for(int i = 0; i < names.size(); ++i){
    pl.push_back(Register::newPlayer(names[i]));
    pl[i]->id_ = i;
  }

  b.iniBoard(seed, rounds, config_file);

  std::cout << rounds << " " << b.info.rows() << " " << b.info.cols() << std::endl;
  std::cout << names.size();
  for(const std::string& s : names) std::cout << " " << s;
  std::cout << std::endl;
  b.printRound(new_viewer);

  crashed.resize(pl.size());
  for (auto&& x : crashed)
    x = false;

  for(int round = 0; round < rounds; ++round){

    b.info.currentRound = round;
    if(debug) std::cerr << "starting round " << round << std::endl << std::endl;
    
    if(parallel)
    {
      for(int i = 0; i < pl.size(); ++i)
      {
        pl[i]->resetList();
      }    
      Timer timerplay("Play timer",&tplay,false);
      {
        int nthreads = std::thread::hardware_concurrency();
        if(pl.size() < nthreads) nthreads = pl.size();
        jaslo::ThreadPool pool(nthreads);

        for(int i = 0; i < pl.size(); ++i)
        {
          pool.enqueue([](Player* p){ p->play();},pl[i]);
        }   
      }
    }
    else
    {
      current_player = 0;
      
      for(int i = current_player; i < pl.size(); ++i){
        current_player = i;
        if (crashed[i])
          continue;
        
        if(debug) std::cerr << "player " << names[i] << std::endl;
        
        pl[i]->resetList();

        Timer timerplay("Play timer",&tplay,false);
        try
        {
          pl[i]->play();
        }
        catch(const std::exception& e)
        {
          if(debug) std::cerr << "player " << names[i] << " crashed! Ignoring next turns" << std::endl;
          crashed[i] = true;
        }
        if(debug) std::cerr << "end player " << names[i] << std::endl << std::endl;
      }
      current_player = -1;
    }

    { 
    Timer timerround("Round timer",&tround,false);
    //Wait for all play() funcs before this
    b.executeRound(pl);
    }
    b.printRound(new_viewer);
    if(debug) std::cerr << "ending round " << round << std::endl << std::endl << std::endl;
  }

  b.printSettings();

  b.printResults(names);

  double totaltime = timerall.getTime();
  std::cerr << "Global timer: " << totaltime << "\nPlay timer: " << tplay << "\nRound timer: " << tround << "\n";
  std::cerr << "play %: " << (tplay/totaltime)*100 << "\nround %: " << (tround/totaltime)*100 << "\n";

}