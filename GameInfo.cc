#include "GameInfo.hh"

//Debug

//Release
int GameInfo::maxRounds;
int GameInfo::currentRound;
int GameInfo::numPlayers;
int GameInfo::unitsStart;
int GameInfo::unitsMax;
int GameInfo::unitsMin;
int GameInfo::bonusMax;
int GameInfo::roundsPerRespawn;
int GameInfo::roundsPerBubble;
int GameInfo::roundsPerBubbleMove;
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
std::vector<std::vector<char>> GameInfo::game_map;
Matrix<Square> GameInfo::square_map;
Matrix<Square> GameInfo::old_square_map;
//vector<vector<Square>> GameInfo::square_map;
//vector<vector<Square>> GameInfo::old_square_map;
std::vector<int> GameInfo::whoHasWhat;  //if v[0] = 1, player 1 has unit with id 0
std::vector<Unit> GameInfo::unitsVector;
std::vector<Bubble> GameInfo::bubblesVector;
std::vector<int> GameInfo::playerPoints;
std::vector<int> GameInfo::bonusPlayers;
std::vector<Bonus> GameInfo::bonusVector;

//Spawn events management
std::vector<std::vector<Position>> GameInfo::player_squares;
std::vector<int> GameInfo::respawnCounters;
std::vector<int> GameInfo::bubbleCounters;
std::vector<int> GameInfo::bubbleMovementCounters;
std::vector<int> GameInfo::bonusCounters;



int GameInfo::cols(){return boardWidth;}
int GameInfo::rows(){return boardHeight;}
int GameInfo::round(){return GameInfo::currentRound;}
int GameInfo::points(int pl){
  if(pl < 0 or pl >= numPlayers){
    std::cerr << "warning: asked for player " << pl << " who does not exist" << std::endl;
    return -1;
  }
  return playerPoints[pl];
}
Square GameInfo::square(const Position& p){
  if(p.y < 0 or p.y > cols() or p.x > rows() or p.x < 0) std::cerr << "Position is not valid (sq)" << std::endl;
  return square_map[p];
}
Unit GameInfo::unit(int uid){
  if(uid < 0 or uid >= unitsVector.size()){std::cerr << "warning: requested for a unit that doesn't exist" << std::endl; exit(1);}
  return unitsVector[uid];
}
Unit GameInfo::unit(Position p){
  if(not posOk(p)){ std::cerr << "warning: position " << p.x << " " << p.y << " is outside the board" << std::endl; exit(1);}
  Square sq = square(p);
  if(sq.u == nullptr){std::cerr << "warning: position " << p.x << " " << p.y << " does not have any unit" << std::endl; exit(1);}
  return sq.unit();
}
Bubble GameInfo::bubble(int bid){
  if(bid < 0 or bid >= bubblesVector.size()){std::cerr << "warning: requested for a bubble that doesn't exist" << std::endl; exit(1);}
  return bubblesVector[bid];
}
Bubble GameInfo::bubble(Position p){
  if(not posOk(p)){ std::cerr << "warning: position " << p.x << " " << p.y << " is outside the board" << std::endl;exit(1);}
  Square sq = square(p);
  if(sq.bb == nullptr){ std::cerr << "warning: position " << p.x << " " << p.y << " does not have any bubble" << std::endl; exit(1); }
  return sq.bubble();
}


void GameInfo::printSettings(){
  std::cout << "NUMBER_OF_PLAYERS" << '\t' << numPlayers << std::endl
  << "UNITS_AT_START" << "\t\t"       << unitsStart << std::endl
  << "MAX_NUMBER_OF_UNITS" << '\t'    << unitsMax << std::endl
  << "MIN_NUMBER_OF_UNITS" << '\t'    << unitsMin << std::endl
  << "MAX_NUMBER_OF_BONUSES" << '\t'  << bonusMax << std::endl
  << "ROUNDS_PER_RESPAWN" << '\t'     << roundsPerRespawn << std::endl
  << "ROUNDS_PER_BUBBLE" << '\t'      << roundsPerBubble << std::endl
  << "ROUNDS_PER_BUBBLE_MOVE"         << roundsPerBubbleMove << std::endl
  << "MAX_ROUNDS_PER_BONUS" << '\t'   << roundsPerBonus << std::endl
  << "POINTS_PER_UNIT" << "\t\t"      << pointsPerUnit << std::endl
  << "POINTS_PER_BUBBLE" << '\t'      << pointsPerBubble << std::endl
  << "POINTS_PER_POP" << "\t\t"       << pointsPerPop << std::endl
  << "POINTS_PER_SQUARE" << '\t'      << pointsPerSquare << std::endl
  << "ENERGY_AT_START" << "\t\t"      << energyStart << std::endl
  << "MAX_ENERGY" << "\t\t"           << energyMax << std::endl
  << "MIN_ENERGY" << "\t\t"           << energyMin << std::endl
  << "ABILITY SIZE" << "\t\t"         << abilitySize << std::endl
  << "ABILITY DURATION" << '\t'       << abilityDuration << std::endl
  << "BOARD_WIDTH" << "\t\t"          << boardWidth << std::endl
  << "BOARD_HEIGHT" << "\t\t"         << boardHeight << std::endl
  << "ROUNDS_TO_POP" << "\t"          << roundsToPop << std::endl;
  //<< "" << '\t' << << endl
}

void GameInfo::readSettings(){
  std::string s; //string to read and discard variable names
  std::cin >> s >> unitsStart 
  >> s >> unitsMax 
  >> s >> unitsMin
  >> s >> bonusMax 
  >> s >> roundsPerRespawn 
  >> s >> roundsPerBubble
  >> s >> roundsPerBubbleMove
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

  whoHasWhat = std::vector<int>(unitsMax*numPlayers,-1);
  playerPoints = std::vector<int>(numPlayers,0);
  bonusPlayers = std::vector<int>(numPlayers,0);
  player_squares = std::vector<std::vector<Position>>(numPlayers);
  bubbleCounters = std::vector<int>(numPlayers,0);
  bubblesVector = std::vector<Bubble>(numPlayers*((maxRounds/roundsPerBubble)+1));
  bubbleMovementCounters = std::vector<int>(bubblesVector.size(),0);
  for(int i = 0; i < bubblesVector.size(); ++i) bubblesVector[i] = Bubble(i,-1,Position(-1,-1),0);
  respawnCounters = std::vector<int>(numPlayers,0);
  bonusVector = std::vector<Bonus>(bonusMax);
  for(int i = 0; i < bonusVector.size(); ++i) bonusVector[i] = Bonus(i,Position(-1,-1));
  bonusCounters = std::vector<int>(bonusMax);
  for(int& c : bonusCounters) c = randomNumber(0,roundsPerBonus)-1;
}

bool GameInfo::posOk(const Position& p){
  if(p.x >= 0 and p.y >= 0 and p.x < boardHeight and p.y < boardWidth) return true;
  //else cerr << "checking position " << p.x << " " << p.y << endl;
  return false;
}

std::vector<int> GameInfo::units(int player){
  std::vector<int> u;
  for(int i = 0; i < unitsVector.size(); ++i){
    if(unitsVector[i].player() == player) u.push_back(i); 
  }
  return u;
}

std::vector<int> GameInfo::numberOfUnits(){
  std::vector<int> n(4,0);
  for(int i = 0; i < unitsVector.size(); ++i){
    int pl = unitsVector[i].player();
    if(pl >= 0 and pl < numPlayers) n[pl]++; 
  }
  return n;
}

bool GameInfo::freeSquare(int plId, Position& p){
  if(plId >= numPlayers){
    std::cerr << "freeSquare requested for player " << plId << ", who does not exist" << std::endl;
    return false;
  }
  int size = player_squares[plId].size();

  std::vector<int> free;
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
    std::cerr << "Player does not exist, didn't spawn unit" << std::endl;
    exit(1);
  }
  int in;
  for(in = 0; in < unitsVector.size(); ++in){
    if(unitsVector[in].player() == -1){ //unit is free
      break;
    }
  }

  if(in == unitsVector.size()){
    std::cerr << "could not find an unused unit, but i'm supposed to" << std::endl;
    exit(1);
  }

  //spawns unit
  Unit u = unitsVector[in];
  u.pl = plId;
  u.p = p;
  u.upg = false;
  u.energ = energyStart;
  unitsVector[in] = u;

  square_map[p].u = &unitsVector[in];
}

void GameInfo::spawnBubble(int plId, Position p){
  if(plId >= numPlayers){
    std::cerr << "Player does not exist, didn't spawn bubble" << std::endl;
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
    std::cerr << "for some reason, bubble vector size is not enough" << std::endl;
    exit(1);
  }
  bubblesVector[in] = b;

  square_map[p].bb = &bubblesVector[in];
}