#include "../game/Player.hh"
using namespace std;

#define PLAYER_NAME Player3

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

  struct BFSInfo{
    Position pos;
    int distance;
    BFSInfo(Position p, int dist){
      pos = p; distance = dist;
    }
  };

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

  template<typename T>
  bool contains(set<T>& set, const T& value){
    return set.find(value) != set.end();
  }

  //Global structures

  const Direction DIRS_DIAGONAL[4] = {Direction::UL, Direction::UR, Direction::DL, Direction::DR};
  const Direction DIRS_STRAIGHT[4] = {Direction::left, Direction::right, Direction::up, Direction::down};
  const Direction DIRS_ALL[8] = {Direction::left, Direction::right, Direction::up, Direction::down,
                                 Direction::UL, Direction::UR, Direction::DL, Direction::DR};

  double time = 0;
  double bfsTime = 0;
  double whileTime = 0;
  double whileValid = 0;
  double timeSet = 0;
  double queueTime = 0;

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

  bool layered_bfs(Position& target, set<Position>& visited, queue<Position>& toVisit, bool(*comp)(const Square&))
  {
    return false;
  }

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

  static bool playerHasBubble(const Square& sq, Position st, BFSInfo inf){
    return sq.hasBubble();
  }

  static bool playerHasBonus(const Square& sq, Position st, BFSInfo inf){
    return sq.hasBonus();
  }

  virtual void play(){
    Timer timer("play()",&time,false);

    //changing player

    //Collect units    
    vector<int> u = units(me());
    /*cerr << "unit list: " << endl;
    for(int un : u){
      cerr << un << " at ";
      cerr << int(unit(un).position().x) << "," << int(unit(un).position().y) << " ";
    }
    cerr << endl;*/

    for(int i = 0; i < u.size(); ++i){
      if(unit(u[i]).upgraded()){
        ability(u[i]);
        continue;
      }
      Position target;
      bool doesbfs = bfs(target,unit(u[i]).position(),playerHasBonus,nullptr,true,me(),30);
      if(doesbfs){
        Position aux = unit(u[i]).position();

        Position vector = Position(target.x-aux.x,target.y-aux.y);

        Direction where = decide(vector);
        move(u[i],where);
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