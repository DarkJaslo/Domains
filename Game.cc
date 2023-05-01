#include "Game.hh"

//Game

Game::Game(){}
void Game::play(const vector<string>& names, int seed, bool showInfo){
  cerr << "starting game with seed " << seed << endl;
  vector<Player*> pl;
  //vector<string> names = {"Player1", "Player2"};
  string s;
  int rounds;
  cin >> s >> rounds;
  Board b(false,true);
  for(int i = 0; i < names.size(); ++i){
    pl.push_back(Register::newPlayer(names[i]));
    pl[i]->id_ = i;
  }

  //Hay que inicializar bien el tablero
  b.iniBoard(seed);


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
  cout << rounds << " " << b.info.rows() << " " << b.info.cols() << endl;
  b.printRound();

  for(int round = 0; round < rounds; ++round){

    b.info.currentRound = round;

    if(showInfo) cerr << "starting round " << round << endl << endl;
    for(int i = 0; i < pl.size(); ++i){
      if(showInfo) cerr << "player " << names[i] << endl;
      pl[i]->resetList();
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