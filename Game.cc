#include "Game.hh"

//Game

Game::Game(){}
void Game::play(const vector<string>& names, bool showInfo){
  vector<Player*> pl;
  //vector<string> names = {"Player1", "Player2"};
  string s;
  int rounds;
  cin >> s >> rounds;
  Board b;
  for(int i = 0; i < names.size(); ++i){
    pl.push_back(Register::newPlayer(names[i]));
    pl[i]->id_ = i;
  }

  if(showInfo) cerr << "initializing board" << endl;
  //Hay que inicializar bien el tablero
  b.iniBoard();
  b.printRound();

  for(int round = 0; round < rounds; ++round){
    if(showInfo) cerr << "starting round " << round << endl << endl;
    for(int i = 0; i < pl.size(); ++i){
      if(showInfo) cerr << "player " << names[i] << endl;
      pl[i]->play();
      if(showInfo) cerr << "end player " << names[i] << endl << endl;
    }

    //Hay que empezar a ejecutar rondas
    b.executeRound(pl);
    b.printRound();
    if(showInfo) cerr << "ending round " << round << endl << endl << endl;
  }

  b.printSettings();

/*order test
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
  */
}