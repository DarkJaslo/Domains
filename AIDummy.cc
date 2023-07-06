#include "Player.hh"
#include <limits>
using namespace std;

#define PLAYER_NAME Dummy

struct PLAYER_NAME : public Player{

static Player* factory(){ return new PLAYER_NAME; }

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

struct Target{
  Target(){
    bonus = false; bubble = false;
  }
  bool targeted()const{ return bonus or bubble; }
  int targetNum;
  bool bonus, bubble;
};

//WIP, computePriority 
class Option{
public:
  enum StepState{ NO, AVOID, NEUTRAL, PREFER};
  enum ContentState{ NOTHING, ENEMY_DRAW, ENEMY_DRAW_HERE, BUBBLE, BUBBLE_HERE, BONUS, BONUS_HERE, FIGHT, FIGHT_HERE, FREE_ATTACK, DIAG_DANGER};
  Option(){
    _sstate = StepState::NEUTRAL;
    _cstate = ContentState::NOTHING;
    _priority = BASE_PRIORITY;
    _dangerous = false;
  }
  bool isDangerous() const{ return _dangerous; }
  bool canStep() const{ return _sstate != StepState::NO; }
  bool hasSomething() const{ return _cstate != ContentState::NOTHING; }
  StepState stepState() const{ return _sstate; }
  ContentState contentState() const{ return _cstate; }
  float priority()const{ return _priority; }

  void setDangerous(bool value){ _dangerous = value; }
  void setStep(StepState state){ _sstate = state; }
  void setContent(ContentState state){ _cstate = state; }
  void setPriority(float value){ _priority = value; }
  void addPriority(float value){ _priority += value; }

  void computePriority(){
    //Sets _priority to a value that reflects the priority of an order here
    //Maybe return an "urgent" attribute too?
  }

private:
  const float BASE_PRIORITY = 100.0f;
  StepState     _sstate;
  ContentState  _cstate;
  float         _priority;
  bool          _dangerous;
};

//WIP  Still doesn't check if things are targeted
class Options{
/*
Stores options for a unit in a 5x5 grid. The process:

Scan: if enemies are found, mark all bad squares that can be attacked by them
      mark all drawn squares of currently drawing units as impossible to step on
      mark the rest of drawings as "please avoid"
      mark enemy drawings as "please step on this"
      if inside an enemy ability, mark all outside squares as impossible to step on
      if outside an enemy ability, mark all inside-of-an-enemy-ability squares as impossible to step on

if the unit's current square is going to be attacked, 
  attack the easiest to defeat enemy
  if none are found (diagonal target) look for other melee interactions
  if none are found, try to get away (see where you can force the unit to exit)
if there's a free attack, perform it
if there's a fair fight, do it (energy?)
if there's a free bubble, kill it
if there's a free bonus and the position is not targeted, take it
*/

public:
  Options(){
    _options = Matrix<Option>(5,5);
    _center = Position(-1,-1);
    _id = -1;
    _unitId = -1;
    _diagonal = false;
    _orderType = OrderType::ability;
    _orderDir = Direction::null;
    _orderCanChange = true;
  }

  void init(int plId, int uid, Position center, Matrix<Target>* ptr){
    _id = plId;
    _unitId = uid; 
    _targetsPtr = ptr;
    if(not posOk(center)){
      std::cerr << "Trying to set an invalid center" << std::endl;
      exit(1);
    }
    _center = center;
    if(_id == -1){ std::cerr << "Id not set" << std::endl; exit(1);} 
    if(square(_center).painter() == _id){
      _diagonal = true;
    }
  }
  /*
    Marks squares:

      1. If they are not valid (cannot be stepped on)
      2. If they are drawn
      3. If they can be attacked by an enemy

    Gets the content of every square in ContentStates
  */
  void scan(){ //Used after init

    bool enemyAbility = square(_center).ability() and square(_center).painter() != _id;
    bool centerIsMine = square(_center).painter() == _id;

    for(int i = _center.x-2; i <= _center.x+2; ++i){
      for(int j = _center.y-2; j <= _center.y+2; ++j){
        Position p(i,j);
        Position auxVector = p-_center;
        Position index((auxVector)+Position(2,2));
        if(not posOk(p)){ _options[index].setStep(Option::StepState::NO); continue;}
        Square sq = square(p);

        if(sq.hasBonus()){
          if(abs(auxVector.x) <= 1 xor abs(auxVector.y) <= 1){
            _options[index].setContent(Option::ContentState::BONUS_HERE);
          }
          else if(centerIsMine and abs(auxVector.x) <= 1 and abs(auxVector.y) <= 1){
            _options[index].setContent(Option::ContentState::BONUS_HERE);
          }
          else _options[index].setContent(Option::ContentState::BONUS);
        }
        else if(sq.hasBubble()){
          if(abs(auxVector.x) <= 1 xor abs(auxVector.y) <= 1){
            _options[index].setContent(Option::ContentState::BUBBLE_HERE);
          }
          else if(centerIsMine and abs(auxVector.x) <= 1 and abs(auxVector.y) <= 1){
            _options[index].setContent(Option::ContentState::BUBBLE_HERE);
          }
          else _options[index].setContent(Option::ContentState::BUBBLE);
        }

        bool enemyAbilityHere = sq.ability() and sq.painter() != _id;
        bool enemyHere = sq.hasUnit() and sq.unit().player()  != _id;

        if(enemyAbility and not enemyAbilityHere){
          _options[index].setStep(Option::StepState::NO);
        }
        else if(not enemyAbility and enemyAbilityHere){
          _options[index].setStep(Option::StepState::NO);
        }
        else if(sq.drawn()){
          if(sq.unitDrawer() == _unitId){
            _options[index].setStep(Option::StepState::NO);
          }
          else if(sq.drawer() == _id){
            _options[index].setStep(Option::StepState::AVOID);
          }
          else if(sq.drawer() != _id){
            _options[index].setStep(Option::StepState::PREFER);
            
            if(abs(auxVector.x) <= 1 xor abs(auxVector.y) <= 1){
              _options[index].setContent(Option::ContentState::ENEMY_DRAW_HERE);
            }
            else if(centerIsMine and abs(auxVector.x) <= 1 and abs(auxVector.y) <= 1){
              _options[index].setContent(Option::ContentState::ENEMY_DRAW_HERE);
            }
            else _options[index].setContent(Option::ContentState::ENEMY_DRAW);
          }
        }
        if(enemyHere){
          
          _options[index].setContent(Option::ContentState::FIGHT);

          if(abs(auxVector.x) == 1 xor abs(auxVector.y) == 1){
            _options[index].setContent(Option::ContentState::FIGHT_HERE);
          }
          if(abs(auxVector.x) == 1 and abs(auxVector.y) == 1){
            if(centerIsMine) _options[index].setContent(Option::ContentState::FREE_ATTACK);
            else if(sq.painter() == sq.unit().player()){
              _options[index].setContent(Option::ContentState::DIAG_DANGER);
            }
          }

          //Mark all attackable squares as dangerous
          vector<Direction> dirs;
          if(sq.painter() == sq.unit().player()){
            dirs = {Direction::left,Direction::right,Direction::up,Direction::down,
                    Direction::UL,Direction::UR,Direction::DL,Direction::DR};
          }
          else dirs = {Direction::left,Direction::right,Direction::up,Direction::down};
          for(const Direction d : dirs){
            Position aux = index+d;
            if(aux.x >= _options.rows() or aux.y >= _options.cols()) continue;
            _options[aux].setDangerous(true);
          }
        }
      }
    }
  }
  void computeOrder(){
    //Does not work yet


    for(int i = 0; i < _options.rows(); ++i){
      for(int j = 0; j < _options.cols(); ++j){
        Position p(i,j);
        _options[p].computePriority();  
      }
    }

    float prio = 0.0f;
    Position mostPrioritary(-1,-1);
    for(int i = 0; i < _options.rows(); ++i){
      for(int j = 0; j < _options.cols(); ++j){
        Position p(i,j);
        float pr = _options[p].priority();
        if(pr > prio){
          prio = pr;
          mostPrioritary = p;
          
        }  
      }
    }
  }
  bool hasUrgentOrder()const{return _orderCanChange == false; }
  OrderType orderType()const{return _orderType;}
  Direction orderDir()const{return _orderDir;}
private:
  const int BASE_VALUE = 100.0f;
  const Position ADJ[24] = //All relative positions in a 5x5 grid excluding (0,0)
    { Position( 0,1), Position( 0,-1), Position( 1 ,0), Position(-1, 0),
      Position( 1,1), Position(-1, 1), Position( 1,-1), Position(-1,-1),
      Position( 0,2), Position( 0,-2), Position( 2, 0), Position(-2, 0),
      Position( 1,2), Position( 1,-2), Position( 2, 1), Position(-2, 1),
      Position(-1,2), Position(-2,-1), Position(-1,-2), Position( 2,-1),
      Position( 2,2), Position(-2, 2), Position( 2,-2), Position(-2,-2) };
  Matrix<Option>  _options;
  Matrix<Target>* _targetsPtr;
  Position        _center;
  int             _id;
  int             _unitId;
  bool            _diagonal;
  OrderType       _orderType;
  Direction       _orderDir;
  bool            _orderCanChange;
};

struct BFSInfo{
  Position pos;
  int dist;
  bool operator<(const BFSInfo& another){
    return dist < another.dist;
  }
  BFSInfo(Position p, int dist) : pos(p), dist(dist){}
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

struct BFSState{
  PositionSet vis;
  queue<BFSInfo> toVis;
  Position start;
  int uid;
};

struct BFSStruct{
  Square sq;
  Position st;
  BFSInfo inf;
  int plId;
  Target target;
  BFSStruct(const Square& sq, Position startPos, const BFSInfo& inf, int plId, Target target) 
    :sq(sq),st(startPos),inf(inf),plId(plId),target(target) {}
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
Matrix<Target> targets;


//Auxiliar functions
static bool playerHasBubble(const BFSStruct& str){ return str.sq.hasBubble(); }
static bool playerHasBonus(const BFSStruct& str){ return str.sq.hasBonus(); }
static bool hasBubbleOrBonus(const BFSStruct& str){ return str.sq.hasBubble() or str.sq.hasBonus(); }
static bool playerHasBubble(const Square& sq){ return sq.hasBubble(); }
static bool allyUnit(const BFSStruct& str){ return str.sq.hasUnit() and str.sq.unit().player() == str.plId; }
static bool allyNoTarget(const BFSStruct& str){ return allyUnit(str);/* and not str.target.targeted();*/ }
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
      toVisit.push(BFSInfo(aux,info.dist+1));
      visited.queue(aux);
    }
  }
}

//Will allow a lot of stuff when completed
bool layered_bfs(Position& target, PositionSet& visited, queue<BFSInfo>& toVisit, 
  bool(*found)(const BFSStruct&),
  bool(*stop) (const BFSStruct&) = nullptr,
    bool tryDiagonal = false, int plId = -1)
{
  int dist = toVisit.front().dist;
  //cerr << " performing layered with " << dist << endl;

  do{
    BFSInfo act = toVisit.front();
    toVisit.pop();
    queueAdjacentPositions(act,toVisit,visited,true,plId);
    if(not posOk(act.pos)) continue;
    if(visited.contains(act.pos)) continue;
    visited.add(act.pos);

    BFSStruct str(square(act.pos),Position(-1,-1),act,plId,targets[act.pos]);
    if(found(str)){
      target = act.pos;
      return true;
    }
    else if(stop != nullptr and stop(str)){
      target = Position(-1,-1);
      return true;
    }
  }
  while(not toVisit.empty() and toVisit.front().dist == dist);
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
  /*for(int i = 0; i < 50; ++i){
    for(int j = 0; j < 50; ++j){
      distances[Position(i,j)] = 0;
    }
  }*/
  queueAdjacentPositions(BFSInfo(start,0),toVisit,visited,tryDiagonal,plId);

  while(not toVisit.empty()){
    BFSInfo info = toVisit.front();
    toVisit.pop();

    if(info.dist > radius) return false;

    if(visited.contains(info.pos)) continue;
    visited.add(info.pos);
    
    //the function would execute here with a continue

    Square sq = square(info.pos);
    BFSStruct str(sq,start,info,plId,targets[info.pos]);
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

void giveImmediateOrders(){

}

void giveBonusOrders(){
  int layeredDist = 20;

  vector<BFSState> layered(bonusPositions.size()+bubblePositions.size());

  for(int i = 0; i < bonusPositions.size(); ++i){
    layered[i].start = bonusPositions[i];
    layered[i].uid = -1;
    BFSInfo inf(layered[i].start,0);
    queueAdjacentPositions(inf,layered[i].toVis,layered[i].vis,true,me());
  }

  for(int i = 0; i < bubblePositions.size(); ++i){
    int index = i+bonusPositions.size();
    layered[index].start = bubblePositions[i];
    layered[index].uid = -1;
    BFSInfo inf(layered[index].start,0);
    queueAdjacentPositions(inf,layered[index].toVis,layered[index].vis,true,me());
  }

  for(int c = 1; c < layeredDist; ++c){
    for(int i = 0; i < layered.size(); ++i){
      if(layered[i].uid != -1) continue;
      BFSState& l = layered[i];
      Position target;
      bool res = layered_bfs(target,l.vis,l.toVis,allyNoTarget,enemyUnit,true,me());
      if(not res) continue;
      if(target == Position(-1,-1)){
        layered[i].uid = -2; //Enemy is going for it
        continue;
      }
      Square sq = square(target);
      int realUid = unit(target).id();
      int uid = findInVector(uns,realUid);
      if(uid == -1) continue;
      if(not orders[uid].canChange) continue;
      bool diagonal = sq.painter() == me();
      Position pos;
      if(i < bonusPositions.size()){
        pos = bonusPositions[i];
      }
      else{
        pos = bubblePositions[i-bonusPositions.size()];
      }
      orders[uid] = Order(uid,bestDirectionFlex(pos,target,diagonal),false,OrderType::movement,distance(pos,target));
      layered[i].uid = uid;
    }
  }
}

void giveDrawOrders(){

}

void executeOrders(){
  for(const Order& ord : orders){
    switch(ord.type){
      case OrderType::movement:
        move(uns[ord.uid],ord.dir);
        break;
      case OrderType::attack:
        attack(uns[ord.uid],ord.dir);
        break;
      case OrderType::ability:
        //cerr << "ESTOY HACIENDO ESTO" << endl;
        ability(uns[ord.uid]);
        break;
    }
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
  targets = Matrix<Target>(rows(),cols());
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
    //uses an available ability
    for(int i = 0; i < uns.size(); ++i){
      Unit u = unit(uns[i]);
      if(u.upgraded()){
        orders[i] = Order(i,Direction::null,false,OrderType::ability,0);
        break;
      }
    }
    //giveImmediateOrders();
    giveBonusOrders();
    //giveDrawOrders();
    executeOrders();
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





