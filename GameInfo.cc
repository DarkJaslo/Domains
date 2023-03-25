#include "GameInfo.hh"


int GameInfo::cols()const{return game_map[0].size();}
int GameInfo::rows()const{return game_map.size();}
Square GameInfo::square(const Position& p)const{
  if(p.y < cols() or p.y > cols() or p.x > rows() or p.x < rows()) cerr << "Position is not valid" << endl;
  return square_map[p.x][p.y];
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
}