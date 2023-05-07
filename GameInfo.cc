#include "GameInfo.hh"

int GameInfo::maxRounds;
int GameInfo::currentRound;
int GameInfo::numPlayers;
int GameInfo::unitsStart;
int GameInfo::unitsMax;
int GameInfo::unitsMin;
int GameInfo::bonusMax;
int GameInfo::roundsPerRespawn;
int GameInfo::roundsPerBubble;
int GameInfo::roundsPerBonus;
int GameInfo::pointsPerUnit;
int GameInfo::pointsPerBubble;
int GameInfo::pointsPerPop;
int GameInfo::pointsPerSquare;
int GameInfo::energyStart;
int GameInfo::energyMax;
int GameInfo::energyMin;
int GameInfo::abilitySize;
int GameInfo::abilityDuration;
int GameInfo::boardWidth;
int GameInfo::boardHeight;
int GameInfo::roundsToPop;
int GameInfo::currentBubbles;
int GameInfo::currentBonuses;
vector<vector<char>> GameInfo::game_map;
vector<vector<Square>> GameInfo::square_map;
vector<vector<Square>> GameInfo::old_square_map;
vector<int> GameInfo::whoHasWhat;  //if v[0] = 1, player 1 has unit with id 0
vector<Unit> GameInfo::unitsVector;
vector<Bubble> GameInfo::bubblesVector;
vector<int> GameInfo::playerPoints;
vector<int> GameInfo::bonusPlayers;
vector<Bonus> GameInfo::bonusVector;

//Spawn events management
vector<vector<Position>> GameInfo::player_squares;
vector<int> GameInfo::respawnCounters;
vector<int> GameInfo::bubbleCounters;
vector<int> GameInfo::bonusCounters;



int GameInfo::cols(){return boardWidth;}
int GameInfo::rows(){return boardHeight;}
int GameInfo::round(){return GameInfo::currentRound;}
int GameInfo::points(int pl){
  if(pl < 0 or pl >= numPlayers){
    cerr << "warning: asked for player " << pl << " who does not exist" << endl;
    return -1;
  }
  return playerPoints[pl];
}
Square GameInfo::square(const Position& p){
  if(p.y < 0 or p.y > cols() or p.x > rows() or p.x < 0) cerr << "Position is not valid (sq)" << endl;
  return square_map[p.x][p.y];
}
Unit GameInfo::unit(int uid){
  if(uid < 0 or uid >= unitsVector.size()){cerr << "warning: requested for a unit that doesn't exist" << endl; exit(1);}
  return unitsVector[uid];
}
Unit GameInfo::unit(Position p){
  if(not posOk(p)){ cerr << "warning: position " << p.x << " " << p.y << " is outside the board" << endl; exit(1);}
  Square sq = square(p);
  if(sq.u == nullptr){cerr << "warning: position " << p.x << " " << p.y << " does not have any unit" << endl; exit(1);}
  return sq.unit();
}
Bubble GameInfo::bubble(int bid){
  if(bid < 0 or bid >= bubblesVector.size()){cerr << "warning: requested for a bubble that doesn't exist" << endl; exit(1);}
  return bubblesVector[bid];
}
Bubble GameInfo::bubble(Position p){
  if(not posOk(p)){ cerr << "warning: position " << p.x << " " << p.y << " is outside the board" << endl;exit(1);}
  Square sq = square(p);
  if(sq.bb == nullptr){ cerr << "warning: position " << p.x << " " << p.y << " does not have any bubble" << endl; exit(1); }
  return sq.bubble();
}


void GameInfo::printSettings(){
  cout << "NUMBER_OF_PLAYERS" << '\t' << numPlayers << endl
  << "UNITS_AT_START" << "\t\t"       << unitsStart << endl
  << "MAX_NUMBER_OF_UNITS" << '\t'    << unitsMax << endl
  << "MIN_NUMBER_OF_UNITS" << '\t'    << unitsMin << endl
  << "MAX_NUMBER_OF_BONUSES" << '\t'  << bonusMax << endl
  << "ROUNDS_PER_RESPAWN" << '\t'     << roundsPerRespawn << endl
  << "ROUNDS_PER_BUBBLE" << '\t'      << roundsPerBubble << endl
  << "MAX_ROUNDS_PER_BONUS" << '\t'   << roundsPerBonus << endl
  << "POINTS_PER_UNIT" << "\t\t"      << pointsPerUnit << endl
  << "POINTS_PER_BUBBLE" << '\t'      << pointsPerBubble << endl
  << "POINTS_PER_POP" << "\t\t"       << pointsPerPop << endl
  << "POINTS_PER_SQUARE" << '\t'      << pointsPerSquare << endl
  << "ENERGY_AT_START" << "\t\t"      << energyStart << endl
  << "MAX_ENERGY" << "\t\t"           << energyMax << endl
  << "MIN_ENERGY" << "\t\t"           << energyMin << endl
  << "ABILITY SIZE" << "\t\t"         << abilitySize << endl
  << "ABILITY DURATION" << '\t'       << abilityDuration << endl
  << "BOARD_WIDTH" << "\t\t"          << boardWidth << endl
  << "BOARD_HEIGHT" << "\t\t"         << boardHeight << endl
  << "ROUNDS_TO_POP" << "\t"          << roundsToPop << endl;
  //<< "" << '\t' << << endl
}

void GameInfo::readSettings(){
  string s; //string to read and discard variable names
  cin >> s >> unitsStart 
  >> s >> unitsMax 
  >> s >> unitsMin
  >> s >> bonusMax 
  >> s >> roundsPerRespawn 
  >> s >> roundsPerBubble
  >> s >> roundsPerBonus
  >> s >> pointsPerUnit 
  >> s >> pointsPerBubble
  >> s >> pointsPerPop 
  >> s >> pointsPerSquare 
  >> s >> energyStart 
  >> s >> energyMax 
  >> s >> energyMin 
  >> s >> abilitySize
  >> s >> abilityDuration
  >> s >> boardWidth
  >> s >> boardHeight
  >> s >> roundsToPop;

  whoHasWhat = vector<int>(unitsMax*numPlayers,-1);
  playerPoints = vector<int>(numPlayers,0);
  bonusPlayers = vector<int>(numPlayers,0);
  player_squares = vector<vector<Position>>(numPlayers);
  bubbleCounters = vector<int>(numPlayers,0);
  bubblesVector = vector<Bubble>(numPlayers*((maxRounds/roundsPerBubble)+1));
  for(int i = 0; i < bubblesVector.size(); ++i) bubblesVector[i] = Bubble(i,-1,Position(-1,-1),0);
  respawnCounters = vector<int>(numPlayers,0);
  bonusVector = vector<Bonus>(bonusMax);
  for(int i = 0; i < bonusVector.size(); ++i) bonusVector[i] = Bonus(i,Position(-1,-1));
  bonusCounters = vector<int>(bonusMax);
  for(int& c : bonusCounters) c = randomNumber(0,roundsPerBonus)-1;
}

bool GameInfo::posOk(const Position& p){
  if(p.x >= 0 and p.y >= 0 and p.x < boardHeight and p.y < boardWidth) return true;
  //else cerr << "checking position " << p.x << " " << p.y << endl;
  return false;
}

vector<int> GameInfo::units(int player){
  vector<int> u;
  for(int i = 0; i < unitsVector.size(); ++i){
    if(unitsVector[i].player() == player) u.push_back(i); 
  }
  return u;
}

vector<int> GameInfo::numberOfUnits(){
  vector<int> n(4,0);
  for(int i = 0; i < unitsVector.size(); ++i){
    int pl = unitsVector[i].player();
    if(pl >= 0 and pl < numPlayers) n[pl]++; 
  }
  return n;
}

bool GameInfo::freeSquare(int plId, Position& p){
  if(plId >= numPlayers){
    cerr << "freeSquare requested for player " << plId << ", who does not exist" << endl;
    return false;
  }
  int size = player_squares[plId].size();

  vector<int> free;
  for(int i = 0; i < size; ++i){
    Position p = player_squares[plId][i];
    if(square(p).empty()) free.push_back(i);
  }

  if(free.size() == 0){
    return false;
  }
  int index = randomNumber(0,free.size()-1);
  p = player_squares[plId][free[index]];
  return true;
}

void GameInfo::spawnUnit(int plId, Position p){
  if(plId >= numPlayers){
    cerr << "Player does not exist, didn't spawn unit" << endl;
    exit(1);
  }
  int in;
  for(in = 0; in < unitsVector.size(); ++in){
    if(unitsVector[in].player() == -1){ //unit is free
      break;
    }
  }

  if(in == unitsVector.size()){
    cerr << "could not find an unused unit, but i'm supposed to" << endl;
    exit(1);
  }

  //spawns unit
  Unit u = unitsVector[in];
  u.pl = plId;
  u.p = p;
  u.upg = false;
  u.energ = energyStart;
  unitsVector[in] = u;

  square_map[p.x][p.y].u = &unitsVector[in];
}

void GameInfo::spawnBubble(int plId, Position p){
  if(plId >= numPlayers){
    cerr << "Player does not exist, didn't spawn bubble" << endl;
    exit(1);
  }
  int in;
  for(in = 0; in < bubblesVector.size(); ++in){
    if(bubblesVector[in].player() == -1){ //bubble is free
      break;
    }
  }

  Bubble b = Bubble(in,plId,p,0);

  if(in == bubblesVector.size()){
    //couldn't find a place, make vector bigger
    cerr << "for some reason, bubble vector size is not enough" << endl;
    exit(1);
  }
  bubblesVector[in] = b;

  square_map[p.x][p.y].bb = &bubblesVector[in];
}