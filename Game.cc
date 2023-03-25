#include "Game.hh"

//Game

Game::Game(){}
void Game::play(const vector<string>& names){
  vector<Player*> pl;
  //vector<string> names = {"Player1", "Player2"};
  int rounds = 200;
  //cin >> rounds;
  Board b;
  for(int i = 0; i < names.size(); ++i){
    pl.push_back(Register::newPlayer(names[i]));
    pl[i]->id_ = i;
  }
  cout << "Playing p0" << endl;
  pl[0]->play();
  cout << "Ords p0" << endl;
  pl[0]->printOrderIds();
  cout << "Ords p1" << endl;
  pl[1]->printOrderIds();
  cout << "Playing p1" << endl;
  pl[1]->play();
  cout << "Ords p1" << endl;
  pl[1]->printOrderIds();
}