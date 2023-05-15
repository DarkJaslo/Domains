#include "Player.hh"
#include "Timer.hh"

#define PLAYER_NAME Player1

struct PLAYER_NAME : public Player{
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

  void queueAdjacentPositions(BFSInfo info, queue<BFSInfo>& toVisit, set<Position>& visited, bool tryDiagonal = false, int plId = -1)
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
      if(posOk(aux) and not contains(visited,aux)){
        if(isDiagonal(permutation[i]) and square(aux).painter() != plId) continue;
        toVisit.push(BFSInfo(aux,info.distance+1));
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


    set<Position> visited;
    visited.insert(start);
    queue<BFSInfo> toVisit;
    queueAdjacentPositions(BFSInfo(start,0),toVisit,visited,tryDiagonal,plId);

    while(not toVisit.empty()){
      BFSInfo info = toVisit.front();
      toVisit.pop();

      if(info.distance > radius) return false;

      if(contains(visited,info.pos)) continue;
      visited.insert(info.pos);
      
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
    cerr << "unit list: " << endl;
    for(int un : u){
      cerr << un << " at ";
      cerr << int(unit(un).position().x) << "," << int(unit(un).position().y) << " ";
    }
    cerr << endl;

    for(int i = 0; i < u.size(); ++i){
      if(unit(u[i]).upgraded()){
        ability(u[i]);
        continue;
      }
      Position target;
      bool doesbfs = bfs(target,unit(u[i]).position(),playerHasBonus,nullptr,true,me(),100);
      if(doesbfs){
        Position aux = unit(u[i]).position();
        if(target.x > aux.x){
          move(u[i],Direction::down);
        }
        else if(target.x < aux.x){
          move(u[i],Direction::up);
        }
        else if(target.y > aux.y){
          move(u[i],Direction::right);
        }
        else if(target.y < aux.y){
          move(u[i],Direction::left);
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