#include "Player.hh"

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
    bool(*found)(const Square&, Position st, BFSInfo inf), 
    bool(*stop) (const Square&, Position st, BFSInfo inf) = nullptr, 
      bool tryDiagonal = false, int plId = -1, int radius = 2*rows())
  {
    set<Position> visited;
    visited.insert(start);
    queue<BFSInfo> toVisit;
    queueAdjacentPositions(BFSInfo(start,0),toVisit,visited,tryDiagonal,plId);

    while(not toVisit.empty()){
      BFSInfo info = toVisit.front();
      toVisit.pop();

      if(contains(visited,info.pos)) continue;
      visited.insert(info.pos);

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

  virtual void play(){

    Position example;

    auto lambda = [](const Square& sq,Position st, BFSInfo inf )->bool{return sq.empty();};
    auto lambdastop = [](const Square& sq,Position st, BFSInfo inf )->bool{return not sq.empty();};
    bool b = bfs(example, Position(0,0),lambda,lambdastop);

    //changing player

    //Collect units    
    vector<int> u = units(me());
    cerr << "u.size() = " << u.size() << endl;
    cerr << "player " << me() << ", printing units:" << endl;
    for(int i = 0; i < u.size(); ++i){
      cerr << u[i] << " ";
    }
    cerr << endl;

    /*if(round() == 0){ //first round, few local options
      move(u[0],Direction::up);
    }
    else if(round() == 1){  //same here
      move(u[0],Direction::UL);
    }*/

    Position bonusPos = Position(12,20);

    if(unit(u[0]).upgraded()){
      cerr << "UNIT IS UPGRADED" << endl;
      ability(u[0]);
    }
    else{
      if(unit(u[0]).position().y < bonusPos.y){
        move(u[0],Direction::right);
      }
      else if(unit(u[0]).position().x > bonusPos.x){
        move(u[0],Direction::up);
      }      
    }

    
  }
};

REGISTER_PLAYER(PLAYER_NAME);