#include "Player.hh"
using namespace std;

#define PLAYER_NAME Dummy

struct PLAYER_NAME : public Player{

class PositionSet{
public:
  enum State{OUTSIDE,QUEUED,INSIDE};
  PositionSet(){
    _container = Matrix<int8_t>(50,50);
    clear();
  }
  bool queued(Position p) const{
    return _container[p] == State::QUEUED;
  }
  bool contains(Position p) const{
    return _container[p] == State::INSIDE;
  }
  void queue(Position p){
    _container[p] = State::QUEUED;
  }
  void add(Position p){
    _container[p] = State::INSIDE;
  }
  void remove(Position p){
    _container[p] = State::OUTSIDE;
  }
  void clear(){
    for(int i = 0; i < 50; ++i){
      for(int j = 0; j < 50; ++j){
        _container[Position(i,j)] = State::OUTSIDE;
      }
    }
  }
  void print(){
    std::cerr << "printing contents..." << std::endl;
    for(int i = 0; i < 50; ++i){
      for(int j = 0; j < 50; ++j){
        Position p(i,j);
        if(_container[p] != State::OUTSIDE){
          std::cerr << i << "," << j << " ";
        }
      }
    }
    std::cerr << std::endl;
  }
private:
  Matrix<int8_t> _container;  
};

static Player* factory(){
  return new PLAYER_NAME;
}

//Maybe unused
template<typename T>
bool contains(set<T>& set, const T& value){
  return set.find(value) != set.end();
}

struct BFSInfo{
  Position pos;
  int distance;
  BFSInfo(Position p, int dist){
    pos = p; distance = dist;
  }
};

//Global structures

const vector<Direction> DIRS_DIAGONAL = {Direction::UL, Direction::UR, Direction::DL, Direction::DR};
const vector<Direction> DIRS_STRAIGHT = {Direction::left, Direction::right, Direction::up, Direction::down};
const vector<Direction> DIRS_ALL = {Direction::left, Direction::right, Direction::up, Direction::down,
                                Direction::UL, Direction::UR, Direction::DL, Direction::DR};

#define SR 27

const Direction UNIT0OPENING[SR] = {
  Direction::UR,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  //Stop
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  //Stop
  Direction::left,
  Direction::left,
  Direction::left,
  Direction::left,
  Direction::left,
  Direction::left,
  Direction::left,
  Direction::left
};
//UNIT1 can get to the desired position with 2 moves at max and it spawns 3 rounds later
Direction UNIT1SPECIAL[2];
const Direction UNIT1OPENING[SR-5] = {
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::left
};
Direction UNIT2SPECIAL[2];
const Direction UNIT2OPENING[SR-8] = {
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::right,
  Direction::down,
  Direction::down,
  Direction::down,
  Direction::down,
  Direction::down,
  Direction::down,
  Direction::down,
  Direction::down,
  Direction::null
};
Direction UNIT3SPECIAL[2];
const Direction UNIT3OPENING[SR-11] = {
  Direction::UR,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::null
};
Direction UNIT4SPECIAL[2];
const Direction UNIT4OPENING[SR-14] = {
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::right,
  Direction::right,
  Direction::right
};
Direction UNIT5SPECIAL[2];
const Direction UNIT5OPENING[SR-17] = {
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right,
  Direction::right
};
const Direction UNIT6OPENING[SR-20] = {
  Direction::UR,
  Direction::UR,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up,
  Direction::up
};
const Direction UNIT7OPENING[SR-22] = {
  Direction::UR,
  Direction::UR,
  Direction::right,
  Direction::up,
  Direction::right
};

double time = 0;
double bfsTime = 0;
double whileTime = 0;
double whileValid = 0;
double timeSet = 0;
double queueTime = 0;

vector<Direction> DIRS_MAP; //So directions work as if you were down-left
Position startingPos;

//Auxiliar functions

static bool playerHasBubble(const Square& sq, Position st, BFSInfo inf){ return sq.hasBubble(); }
static bool playerHasBonus(const Square& sq, Position st, BFSInfo inf){ return sq.hasBonus(); }
static bool playerHasBubble(const Square& sq){ return sq.hasBubble(); }
bool isDiagonal(Direction d){return d >= Direction::UL;}
Direction decide(Position pos){
  if(pos.x < 0 and pos.y < 0){
    int rand = randomNumber(0,1);
    if(rand == 0){return Direction::up;}
    return Direction::left;
  }
  else if(pos.x > 0 and pos.y > 0){
    int rand = randomNumber(0,1);
    if(rand == 0){return Direction::down;}
    return Direction::right;
  }
  else if(pos.x == 0){
    if(pos.y < 0){
      return Direction::left;
    }
    return Direction::right;
  }
  else if(pos.y == 0){
    if(pos.x < 0){
      return Direction::up;
    }
    return Direction::down;
  }
  else if(pos.x < 0){
    int rand = randomNumber(0,1);
    if(rand == 0){return Direction::up;}
    return Direction::right;
  }
  else if(pos.y < 0){
    int rand = randomNumber(0,1);
    if(rand == 0){return Direction::down;}
    return Direction::left;
  }
  return Direction::null;
}
Position virtualPos(Position pos){
  cerr << "me: " << me() << endl;
  switch(me()){
    case 1:{
      return Position(rows()-1-pos.x,cols()-1-pos.y);
    }
    case 2:{ //Falta
      return Position(pos.y,cols()-1-pos.x);
    }
    case 3:{                                                        
      return Position(rows()-1-pos.y, pos.x);
    }
    default:{
      return pos;
    }
  }
}

//Assumes diagonal moves are always legal
Direction bestDirection(Position desired, Position act){
  if(act == desired) return Direction::null;
  else if(desired.x > act.x and desired.y > act.y){return Direction::DR;}
  else if(desired.x < act.x and desired.y < act.y){return Direction::UL;}
  else if(desired.x > act.x and desired.y < act.y){return Direction::DL;}
  else if(desired.x < act.x and desired.y > act.y){return Direction::UR;}
  else if(desired.x > act.x){ return Direction::down; }
  else if(desired.x < act.x){ return Direction::up; }
  else if(desired.y > act.y){ return Direction::right; }
  return Direction::left;
}

//Used in openings
void fillSpecialDirections(Position desired, Position act, Direction& spec1, Direction& spec2){
  //Position is virtual
  //desired = virtualPos(desired);
  cerr << "desired: " << desired << endl;
  cerr << "act (pre): " << act << endl;
  act = virtualPos(act);
  cerr << "act: " << act << endl;
  
  spec1 = bestDirection(desired,act);
  cerr << "spec1: " << spec1 << endl;
  act +=spec1;
  cerr << "act: " << act << endl;
  spec2 = bestDirection(desired,act);
  cerr << "spec2: " << spec2 << endl;
  act +=spec2;
  cerr << "act2: " << act << endl;
}

Position meleeTarget(Position act, bool diag, bool(*found)(const Square&))
{
  vector<Direction> dirs;
  if(diag) dirs = DIRS_ALL;
  else dirs = DIRS_STRAIGHT;

  for(int i = 0; i < dirs.size(); ++i){
    Position aux = act+dirs[i];
    if(not posOk(aux)) continue;
    Square sq = square(aux);
    if(found(sq)){
      return aux;
    }
  }

  return Position(-1,-1);
}

//Used in BFS
void queueAdjacentPositions(BFSInfo info, queue<BFSInfo>& toVisit, PositionSet& visited, bool tryDiagonal = false, int plId = -1)
{
  vector<Direction> permutation;

  if(tryDiagonal){
    if(plId == -1){cerr << "using tryDiagonal with plId == -1" << endl; exit(1);}
    permutation.reserve(8);
    vector<int> perm = randomPermutation();
    for(int i = 0; i < perm.size(); ++i){
      permutation.emplace_back(DIRS_DIAGONAL[perm[i]]);
    }
  }
  else{
    permutation.reserve(4);
  }

  vector<int> permNormal = randomPermutation();
  for(int i = 0; i < permNormal.size(); ++i){
    permutation.emplace_back(DIRS_STRAIGHT[permNormal[i]]);
  }
  for(int i = 0; i < permutation.size(); ++i){
    Position aux = info.pos+permutation[i];
    if(posOk(aux) and not visited.queued(aux) and not visited.contains(aux) ){
      if(isDiagonal(permutation[i]) and square(aux).painter() != plId) continue;
      toVisit.push(BFSInfo(aux,info.distance+1));
      visited.queue(aux);
    }
  }
}

//Will allow a lot of stuff when completed
bool layered_bfs(Position& target, set<Position>& visited, queue<Position>& toVisit, bool(*comp)(const Square&))
{
  return false;
}

//Good ol' generic BFS
bool bfs(Position& target, Position start, 
  bool(*found)(const Square&, Position, BFSInfo), 
  bool(*stop) (const Square&, Position, BFSInfo) = nullptr, 
    bool tryDiagonal = false, int plId = -1, int radius = 2*rows())
{
  //Might want to add another function for after posOk()

  PositionSet visited;
  visited.add(start);
  queue<BFSInfo> toVisit;
  Matrix<int> distances(50,50);
  for(int i = 0; i < 50; ++i){
    for(int j = 0; j < 50; ++j){
      distances[Position(i,j)] = 0;
    }
  }
  queueAdjacentPositions(BFSInfo(start,0),toVisit,visited,tryDiagonal,plId);

  while(not toVisit.empty()){
    BFSInfo info = toVisit.front();
    toVisit.pop();

    if(info.distance > radius) return false;

    if(visited.contains(info.pos)) continue;
    visited.add(info.pos);
    
    //the function would execute here with a continue

    Square sq = square(info.pos);
    if(found(sq,start,info)){
      target = info.pos;
      return true;
    }
    else if(stop != nullptr and stop(sq,start,info)){
      return false;
    }
    queueAdjacentPositions(info,toVisit,visited,tryDiagonal,plId);
  }
  return false;
}





void initializeDirections(){
  DIRS_MAP = vector<Direction>(9);
  DIRS_MAP[0] = Direction::null;

  switch(me()){
    case 0:{  //Down-left
      DIRS_MAP[Direction::up   ] = Direction::up;
      DIRS_MAP[Direction::down ] = Direction::down;
      DIRS_MAP[Direction::left ] = Direction::left;
      DIRS_MAP[Direction::right] = Direction::right;
      DIRS_MAP[Direction::UL   ] = Direction::UL;
      DIRS_MAP[Direction::UR   ] = Direction::UR;
      DIRS_MAP[Direction::DL   ] = Direction::DL;
      DIRS_MAP[Direction::DR   ] = Direction::DR;
      startingPos = Position(rows()-2,1);
      break;
    }
    case 1:{  //Up-right
      DIRS_MAP[Direction::up   ] = Direction::down;
      DIRS_MAP[Direction::down ] = Direction::up;
      DIRS_MAP[Direction::left ] = Direction::right;
      DIRS_MAP[Direction::right] = Direction::left;
      DIRS_MAP[Direction::UL   ] = Direction::DR;
      DIRS_MAP[Direction::UR   ] = Direction::DL;
      DIRS_MAP[Direction::DL   ] = Direction::UR;
      DIRS_MAP[Direction::DR   ] = Direction::UL;
      startingPos = Position(1,cols()-2);
      break;
    }
    case 2:{  //Down-right
      DIRS_MAP[Direction::up   ] = Direction::left;
      DIRS_MAP[Direction::down ] = Direction::right;
      DIRS_MAP[Direction::left ] = Direction::down;
      DIRS_MAP[Direction::right] = Direction::up;
      DIRS_MAP[Direction::UL   ] = Direction::DL;
      DIRS_MAP[Direction::UR   ] = Direction::UL;
      DIRS_MAP[Direction::DL   ] = Direction::DR;
      DIRS_MAP[Direction::DR   ] = Direction::UR;
      startingPos = Position(rows()-2,cols()-2);
      break;
    }
    case 3:{  //Up-left
      DIRS_MAP[Direction::up   ] = Direction::right;
      DIRS_MAP[Direction::down ] = Direction::left;
      DIRS_MAP[Direction::left ] = Direction::up;
      DIRS_MAP[Direction::right] = Direction::down;
      DIRS_MAP[Direction::UL   ] = Direction::UR;
      DIRS_MAP[Direction::UR   ] = Direction::DR;
      DIRS_MAP[Direction::DL   ] = Direction::UL;
      DIRS_MAP[Direction::DR   ] = Direction::DL;
      startingPos = Position(1,1);
      break;
    }
    default:
      break;
  }
}


void initialize(){
  initializeDirections();
}

//Play method (keep as short as possible)
virtual void play(){
  Timer timer("play()",&time,false);

  if(round() == 0){ //first round
    initialize();
  }

  //Collect units    
  vector<int> uns = units(me());
  cerr << "unit list: " << endl;
  for(int un : uns){
    cerr << un << " at ";
    cerr << int(unit(un).position().x) << "," << int(unit(un).position().y) << " ";
  }
  cerr << endl;

  if(round() < SR){
    
    //Unit 1
    if(round() >= 3){
      if(round() == 3){
        fillSpecialDirections(Position(49,2),unit(uns[1]).position(),UNIT1SPECIAL[0],UNIT1SPECIAL[1]);
        move(uns[1],DIRS_MAP[UNIT1SPECIAL[0]]);
      }
      else if(round() == 4){  move(uns[1],DIRS_MAP[UNIT1SPECIAL[1]]);  }
      else{  move(uns[1],DIRS_MAP[UNIT1OPENING[round()-5]]);  }
    }

    //Unit 2
    if(round() >= 6){
      if(round() == 6){
        fillSpecialDirections(Position(47,1),unit(uns[2]).position(),UNIT2SPECIAL[0],UNIT2SPECIAL[1]);
        move(uns[2],DIRS_MAP[UNIT2SPECIAL[0]]);
      }
      else if(round() == 7){  move(uns[2],DIRS_MAP[UNIT2SPECIAL[1]]);  }
      else{  move(uns[2],DIRS_MAP[UNIT2OPENING[round()-8]]);  }
    }

    //Unit 3
    if(round() >= 9){
      if(round() == 9){
        fillSpecialDirections(Position(47,2),unit(uns[3]).position(),UNIT3SPECIAL[0],UNIT3SPECIAL[1]);
        move(uns[3],DIRS_MAP[UNIT3SPECIAL[0]]);
      }
      else if(round() == 10){  move(uns[3],DIRS_MAP[UNIT3SPECIAL[1]]);  }
      else{  move(uns[3],DIRS_MAP[UNIT3OPENING[round()-11]]);  }
    }

    //Unit 4
    if(round() >= 12){
      if(round() == 12){
        fillSpecialDirections(Position(47,0),unit(uns[4]).position(),UNIT4SPECIAL[0],UNIT4SPECIAL[1]);
        move(uns[4],DIRS_MAP[UNIT4SPECIAL[0]]);
      }
      else if(round() == 13){  move(uns[4],DIRS_MAP[UNIT4SPECIAL[1]]);  }
      else{  move(uns[4],DIRS_MAP[UNIT4OPENING[round()-14]]);  }
    }

    //Unit 5
    if(round() >= 15){
      if(round() == 15){
        fillSpecialDirections(Position(48,2),unit(uns[5]).position(),UNIT5SPECIAL[0],UNIT5SPECIAL[1]);
        move(uns[5],DIRS_MAP[UNIT5SPECIAL[0]]);
      }
      else if(round() == 16){  move(uns[5],DIRS_MAP[UNIT5SPECIAL[1]]);  }
      else{  move(uns[5],DIRS_MAP[UNIT5OPENING[round()-17]]);  }
    }

    //Unit 6
    if(round() >= 18){
      if(round() == 18){
        move(uns[6],DIRS_MAP[virtualPos(unit(uns[6]).position()).to(Position(48,1))]);
      }
      else if(round() == 19){
        //attack bubble
        Position aux = unit(uns[6]).position();
        Position targetPos = meleeTarget(aux,true,playerHasBubble);
        cerr << "aux = " << aux << endl;
        cerr << "targetPos = " << targetPos << endl;
        if(targetPos != Position(-1,-1)){
          attack(uns[6],aux.to(targetPos));
        }
      }
      else{  move(uns[6],DIRS_MAP[UNIT6OPENING[round()-20]]);  }
    }

    //Unit 7
    if(round() >= 21){
      if(round() == 21){
        move(uns[7],DIRS_MAP[virtualPos(unit(uns[7]).position()).to(Position(48,1))]);
      }
      else{  move(uns[7],DIRS_MAP[UNIT7OPENING[round()-22]]); }
    }

    //Unit 8



    move(uns[0],DIRS_MAP[UNIT0OPENING[round()]]);
  }
  else if(round() == SR){
    move(uns[2],DIRS_MAP[Direction::down]);
    move(uns[0],DIRS_MAP[Direction::left]);
    move(uns[4],DIRS_MAP[Direction::down]);
    move(uns[1],DIRS_MAP[Direction::left]);
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  if(round() == 499){
    cerr << "time for PLAYER1: " << time << endl;
    cerr << "bfs time: " << bfsTime << endl;
    cerr << "while time: " << whileTime << endl;
    cerr << "valid while: " << whileValid << endl;
    cerr << "set time: " << timeSet  << endl;
    cerr << "queue time: " << queueTime << endl;
  }    
}
};

REGISTER_PLAYER(PLAYER_NAME);

/*
  Dummy strategy:
  "Greedy" mindset, goes for the nearest interaction without thinking too much

  Stores targets, so two units don't go after the same thing
  Does a simple opening



  27 ROUND OPENING

  8 units

  0 does 26
  1 does 22 eff
  2 19
  3 16
  4 13
  5 10
  6 7
  7 4
  8 1


  1
  2   
  3
  4
  5
  6
  7
  8
  9
  10
  11
  12
  13
  14
  15
  16
  17
  18
  19
  20
  21
  22
  23
  24
  25
  26
  27
*/





