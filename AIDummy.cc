#include "Player.hh"
#include <limits>
using namespace std;

#define PLAYER_NAME Dummy

struct PLAYER_NAME : public Player{

#define MAXINT numeric_limits<int>::max()

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

struct Order{
  int uid;
  Direction dir;
  bool canChange;
  OrderType type;
  int dist;
  Order(                  ) : uid(-1), dir(null), canChange(true), type(OrderType::movement), dist(MAXINT){}
  Order(int id, Direction dir, bool canChange, OrderType type, int dist) : uid(id), dir(dir ), canChange(canChange), type(type), dist(dist){}
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

vector<int> uns;
vector<Position> bonusPositions;
vector<Position> bubblePositions;
vector<Order> orders;


//Auxiliar functions
struct BFSStruct{
  Square sq;
  Position st;
  BFSInfo inf;
  int plId;
  BFSStruct(const Square& sq, Position startPos, const BFSInfo& inf, int plId) 
    :sq(sq),st(startPos),inf(inf),plId(plId) {}
};
static bool playerHasBubble(const BFSStruct& str){ return str.sq.hasBubble(); }
static bool playerHasBonus(const BFSStruct& str){ return str.sq.hasBonus(); }
static bool playerHasBubble(const Square& sq){ return sq.hasBubble(); }
static bool allyUnit(const BFSStruct& str){ return str.sq.hasUnit() and str.sq.unit().player() == str.plId; }
static bool enemyUnit(const BFSStruct& str){ return str.sq.hasUnit() and str.sq.unit().player() != str.plId; }
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
  //cerr << "me: " << me() << endl;
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
template<typename T>
int findInVector(const vector<T>& vec, const T& thing){
  for(int i = 0; i < vec.size(); ++i){
    if(vec[i] == thing) return i;
  }
  return -1;
}
int distance(Position pos, Position target){
  return abs(target.x-pos.x)+abs(target.y-pos.y);
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

Direction bestDirectionFlex(Position desired, Position act, bool diag = false){
  if(diag) return bestDirection(desired,act);
  return decide(Position(desired.x-act.x,desired.y-act.y));
}

//Used in openings
void fillSpecialDirections(Position desired, Position act, Direction& spec1, Direction& spec2){
  //Position is virtual
  //desired = virtualPos(desired);
  act = virtualPos(act);
  
  spec1 = bestDirection(desired,act);
  act +=spec1;
  spec2 = bestDirection(desired,act);
  act +=spec2;
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
bool layered_bfs(Position& target, PositionSet& visited, queue<Position>& toVisit, bool(*comp)(const Square&))
{
  return false;
}

//Good ol' generic BFS
bool bfs(Position& target, Position start, 
  bool(*found)(const BFSStruct&), 
  bool(*stop) (const BFSStruct&) = nullptr, 
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
    BFSStruct str(sq,start,info,plId);
    if(found(str)){
      target = info.pos;
      return true;
    }
    else if(stop != nullptr and stop(str)){
      return false;
    }
    queueAdjacentPositions(info,toVisit,visited,tryDiagonal,plId);
  }
  return false;
}


//Stores info about the current map
void scanMap(){
  bonusPositions.clear();
  bubblePositions.clear();

  for(int i = 0; i < rows(); ++i){
    for(int j = 0; j < cols(); ++j){
      Position pos(i,j);
      Square sq = square(pos);
      if(sq.hasBonus()){
        bonusPositions.push_back(pos);
      }
      else if(sq.hasBubble()){
        bubblePositions.push_back(pos);
      }
    }
  }
}

void giveBonusOrders(){
  for(const Position& pos : bonusPositions){
    Position result(-1,-1);
    bool findPlayer = bfs(result,pos,PLAYER_NAME::allyUnit,PLAYER_NAME::enemyUnit,false,me(),30);
    if(not findPlayer) continue;
    //Found a player
    Square sq = square(result);
    int realUid = unit(result).id();
    int uid = findInVector(uns,realUid);
    if(uid == -1) continue;
    if(not orders[uid].canChange) continue;
    if(orders[uid].dist < distance(pos,result)) continue;
    bool diagonal = sq.painter() == me();
    orders[uid] = Order(uid,bestDirectionFlex(pos,result,diagonal),true,OrderType::movement,distance(pos,result));
  }
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

void update(){
  orders = vector<Order>(uns.size());
  for(int i = 0; i < orders.size(); ++i){
    orders[i] = Order(i,Direction::null,true,OrderType::movement,MAXINT);
  }
}

//Play method (keep as short as possible)
virtual void play(){
  Timer timer("play()",&time,false);

  if(round() == 0){ //first round
    initialize();
  }

  //Collect units    
  uns = units(me());
  update();
  /*cerr << "unit list: " << endl;
  for(int un : uns){
    cerr << un << " at ";
    cerr << int(unit(un).position().x) << "," << int(unit(un).position().y) << " ";
  }
  cerr << endl;*/

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
        if(targetPos != Position(-1,-1)){
          attack(uns[6],aux.to(targetPos));
        }
      }
      else{  move(uns[6],DIRS_MAP[UNIT6OPENING[round()-20]]);  }
    }

    //Unit 7
    if(round() >= 21){
      Position aux = virtualPos(unit(uns[7]).position());
      if(aux.x < 47 or aux.y > 2){
        move(uns[7],DIRS_MAP[Direction::null]);
      }
      else{
        if(round() == 21){
          move(uns[7],DIRS_MAP[virtualPos(unit(uns[7]).position()).to(Position(48,1))]);
        }
        else{  move(uns[7],DIRS_MAP[UNIT7OPENING[round()-22]]); }
      }
    }

    //Unit 0
    move(uns[0],DIRS_MAP[UNIT0OPENING[round()]]);
  }
  else if(round() == SR){
    //Very specific moves with a very specific order
    if(virtualPos(unit(uns[8]).position()) == Position(47,2)){
      move(uns[8],DIRS_MAP[Direction::down]);
    }
    else if(virtualPos(unit(uns[9]).position()) == Position(47,2)){
      if(virtualPos(unit(uns[8]).position()) == Position(48,2)){
        move(uns[8],DIRS_MAP[Direction::down]);
      }
      move(uns[9],DIRS_MAP[Direction::down]);
    }
    move(uns[2],DIRS_MAP[Direction::down]);
    move(uns[0],DIRS_MAP[Direction::left]);
    move(uns[4],DIRS_MAP[Direction::down]);
    move(uns[1],DIRS_MAP[Direction::left]);
  }
  else{
    scanMap();
    giveBonusOrders();

    for(const Order& ord : orders){
      switch(ord.type){
        case OrderType::movement:
          move(uns[ord.uid],ord.dir);
          break;
        case OrderType::attack:
          attack(uns[ord.uid],ord.dir);
          break;
        case OrderType::ability:
          ability(uns[ord.uid]);
          break;
      }
    }
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
  Does not use diagonal movement

  Stores targets, so two units don't go after the same thing
  Does a simple opening



  27 round opening


  Strategy:
  units look for critical cases (distance 1-2-diagonals). They always attack when they can.

  from bonuses, look for the nearest player (with a margin M).
    If found, player will go for them. This is checked every round
  
  from any possible bubbles, look for players in a radius. Same as with bonuses

  Do a distance 5 (example) BFS and decide: always combat if found
  If no one is near, do a 9 step drawing
    which has to be complex of course




  In code:

  Solve short distance cases
  Store who has been ordered

  Scan the board and store bonuses and bubbles
  Search from all bonuses
  Search from all bubbles
  Do the BFS

*/





