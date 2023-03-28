#include "GameInfo.hh"


int GameInfo::cols()const{return game_map[0].size();}
int GameInfo::rows()const{return game_map.size();}
Square GameInfo::square(const Position& p)const{
  if(p.y < cols() or p.y > cols() or p.x > rows() or p.x < rows()) cerr << "Position is not valid" << endl;
  return square_map[p.x][p.y];
}

void GameInfo::printSettings(){
  cout << "NUMBER_OF_PLAYERS" << '\t' << numPlayers << endl
  << "UNITS_AT_START" << "\t\t"       << unitsStart << endl
  << "MAX_NUMBER_OF_UNITS" << '\t'    << unitsMax << endl
  << "MIN_NUMBER_OF_UNITS" << '\t'    << unitsMin << endl
  << "MAX_NUMBER_OF_SQUARES" << '\t'  << squaresMax<< endl
  << "MAX_NUMBER_OF_BONUSES" << '\t'  << bonusMax << endl
  << "ROUNDS_PER_RESPAWN" << '\t'     << roundsPerRespawn << endl
  << "POINTS_PER_UNIT" << "\t\t"      << pointsPerUnit << endl
  << "POINTS_PER_BUBBLE" << '\t'      << pointsPerBubble << endl
  << "POINTS_PER_POP" << "\t\t"       << pointsPerPop << endl
  << "POINTS_PER_SQUARE" << '\t'      << pointsPerSquare << endl
  << "ENERGY_AT_START" << "\t\t"      << energyStart << endl
  << "MAX_ENERGY" << "\t\t"           << energyMax << endl
  << "MIN_ENERGY" << "\t\t"           << energyMin << endl
  << "ABILITY SIZE" << "\t\t"         << abilitySize << endl
  << "BOARD_WIDTH" << "\t\t"          << boardWidth << endl
  << "BOARD_HEIGHT" << "\t\t"         << boardHeight << endl;
  //<< "" << '\t' << << endl
}

void GameInfo::readSettings(){
  string s; //string to read and discard variable names
  cin >> s >> numPlayers 
  >> s >> unitsStart 
  >> s >> unitsMax 
  >> s >> unitsMin 
  >> s >> squaresMax 
  >> s >> bonusMax 
  >> s >> roundsPerRespawn 
  >> s >> pointsPerUnit 
  >> s >> pointsPerBubble 
  >> s >> pointsPerPop 
  >> s >> pointsPerSquare 
  >> s >> energyStart 
  >> s >> energyMax 
  >> s >> energyMin 
  >> s >> abilitySize
  >> s >> boardWidth
  >> s >> boardHeight;

  whoHasWhat = vector<int>(unitsMax*numPlayers,-1);
  points = vector<int>(numPlayers,0);
  bonusPlayers = vector<int>(numPlayers,0);
}

bool GameInfo::posOk(const Position& p)const{
  return p.x >= 0 and p.y >= 0 and p.x < boardHeight and p.y < boardWidth;
}

int GameInfo::painter(const Position& p)const{
  if(posOk(p)) return square_map[p.x][p.y].painter();
  else{
    cerr << "error: position (" << p.x << "," << p.y << ") is not valid" << endl;
    return -1;
  }
}

int GameInfo::drawerPlayer(const Position& p)const{
  if(posOk(p)) return square_map[p.x][p.y].drawer();
  else{
    cerr << "error: position (" << p.x << "," << p.y << ") is not valid" << endl;
    return -1;
  }
}

vector<int> GameInfo::units(int player)const{

}