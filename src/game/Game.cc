#include "Game.hh"
#include "ThreadPool.h"
#include <thread>
#include <unistd.h>

//Game

Game::Game(){}
Game::~Game(){
  std::cerr << "deleting game" << std::endl;
}
void Game::play(const std::vector<std::string>& names, int seed, bool fullDebug, bool view, bool parallel, bool debug){
  
  std::cerr << "starting game with seed " << seed << std::endl;
  std::vector<Player*> pl;
  std::string s;
  int rounds;
  std::cin >> s >> rounds;
  Board b(fullDebug,view,names.size());
  for(int i = 0; i < names.size(); ++i){
    pl.push_back(Register::newPlayer(names[i]));
    pl[i]->id_ = i;
  }

  b.iniBoard(seed,rounds);

  /* CODE TO TEST THE RANDOMIZER
  
  int ntests = 1000000;
  int q1,q2,q3,q4;
  q1 = q2 = q3 = q4 = 0;
  int left = 0;
  int right = 1;
  vector<int> v(right+1,0);
  for(int i = 0; i < ntests; ++i){
    int r = GameInfo::randomNumber(left,right);
    //cout << "random number: " << r << endl;
    v[r]++;
    if(r >= left and r <= (left+(right-left)/4)) ++q1;
    else if(r > (left+(right-left)/4) and r <= (left+(right-left)/2)) ++q2;
    else if(r > (left+(right-left)/2) and r <= (left+3*(right-left)/4)) ++q3;
    else ++q4;
  }

  int imax = 0;
  int imin = 0;
  for(int i = left; i < v.size(); ++i){
    if(v[i] > v[imax]) imax = i;
    if(v[i] < v[imin]) imin = i;
    cout << i << " : " << v[i]/double(ntests) << endl;
  }
  cout << "imax : " << imax << " " << v[imax]/double(ntests) << "    imin: " << imin << " " << v[imin]/double(ntests) << endl;
  cout << "q1: " << q1 << " q2: " << q2 << " q3: " << q3 << " q4: " << q4 << endl;
  int qmax = 0;
  int qmin = ntests;
  if(q1 > qmax) qmax = q1;
  if(q2 > qmax) qmax = q2;
  if(q3 > qmax) qmax = q3;
  if(q4 > qmax) qmax = q4;

  if(q1 < qmin) qmin = q1;
  if(q2 < qmin) qmin = q2;
  if(q3 < qmin) qmin = q3;
  if(q4 < qmin) qmin = q4;

  //int errorv0 = qmax-qmin;
  //cout << "errorv0: "<<errorv0<<endl;
  double error = (qmax-qmin)/double(ntests);
  cout << "error: " << error*100 << "%" << endl;

  */

  std::cout << rounds << " " << b.info.rows() << " " << b.info.cols() << std::endl;
  std::cout << names.size();
  for(const std::string& s : names) std::cout << " " << s;
  std::cout << std::endl;
  b.printRound();

  for(int round = 0; round < rounds; ++round){

    b.info.currentRound = round;
    if(debug) std::cerr << "starting round " << round << std::endl << std::endl;
    
    if(parallel)
    {
      for(int i = 0; i < pl.size(); ++i)
      {
        pl[i]->resetList();
      }    
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
      for(int i = 0; i < pl.size(); ++i){
        if(debug) std::cerr << "player " << names[i] << std::endl;
        pl[i]->resetList();
        pl[i]->play();
        if(debug) std::cerr << "end player " << names[i] << std::endl << std::endl;
      }
    }

    //Wait for all play() funcs before this
    b.executeRound(pl);
    b.printRound();
    if(debug) std::cerr << "ending round " << round << std::endl << std::endl << std::endl;
  }

  b.printSettings();
}