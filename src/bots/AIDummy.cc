#include "../game/Player.hh"
#include <functional>

#define PLAYER_NAME Dummy
using namespace std;

struct PLAYER_NAME : public Player{

static Player* factory(){ return new PLAYER_NAME; }

/*

	CLASSES

*/

/* Ad hoc set that greatly improves BFS performance */
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

class Targets{
public:
	Targets(){
		info = Matrix<int8_t>(rows(),cols());
	}

	void clear()
	{
		for(int i = 0; i < rows(); ++i)
			for(int j = 0; j < cols(); ++j)
				info[Position(i,j)] = 0;
	}
	void target(Position p){ info[p]++; }
	bool targeted(Position p) const { return info[p] > 0; }
	int  targets(Position p) const{ return info[p]; }

private:
	Matrix<int8_t> info;
};

struct BFSNode{
	BFSNode(){ dist = -1; pos = Position(-1,-1); }
	BFSNode(int dist, Position pos) : dist(dist), pos(pos) {}
	int dist;
	Position pos;
};

struct BFSInfo{
	BFSInfo(){}
	queue<BFSNode> q;
	PositionSet v;
};

struct MyOrder{
	MyOrder(){prio = 100;}
	MyOrder(const Order& order, int priority) : ord(order), prio(priority){}
	Order ord;
	int prio;

	bool operator<(const MyOrder& other)const
	{
		return prio < other.prio;
	}
};

/*
	Opening moves
*/
const int OPENING_ROUNDS = 27;

vector<Direction> UNIT0OPENING;
vector<Direction> UNIT1OPENING;
vector<Direction> UNIT2OPENING;
vector<Direction> UNIT3OPENING;
vector<Direction> UNIT4OPENING;
vector<Direction> UNIT5OPENING;
vector<Direction> UNIT6OPENING;
vector<Direction> UNIT7OPENING;

Direction UNIT1SPECIAL[2];
Direction UNIT2SPECIAL[2];
Direction UNIT3SPECIAL[2];
Direction UNIT4SPECIAL[2];
Direction UNIT5SPECIAL[2];

/*
	General constants
*/

const vector<Direction> DIRS_DIAGONAL = {Direction::UL, Direction::UR, Direction::DL, Direction::DR};
const vector<Direction> DIRS_STRAIGHT = {Direction::left, Direction::right, Direction::up, Direction::down};
const vector<Direction> DIRS_ALL = {Direction::left, Direction::right, Direction::up, Direction::down,
                                Direction::UL, Direction::UR, Direction::DL, Direction::DR};
vector<Direction> DIRS_MAP; //So directions work as if you were down-left

const int PRIO_MAX = 110;
const int PRIO_FETCH = 50;
const int PRIO_ABILITY = -10; //Currently the most prioritary thing

const int MAX_BFS = 20;
const int MAX_TARGETS = 1;

const int DIST_COMBAT = 2;


/*
	General structures
*/
Position startingPos;
vector<int> myUnits;
vector<bool> availableUnits;
vector<int> commitedUnits;

Targets targets;
vector<MyOrder> orders;

void play()
{
	if(round() == 0)
	{
		init();
	}
	else update();

	doOpeningMoves();

	if(round() <= OPENING_ROUNDS) return;
	//Else,

	/*
	To free units:
	1.Layered bfs of depth D, choose
		-Bubble
		-Bonus
		-Combat
		
	To (still) free units:
	2.Try to draw
	*/

	// Use an ability if available
	useAbility();
	// Gives useful orders to some units
	layeredBFS(1,MAX_BFS);
	// Remaining units should draw
	doDrawingStuff();


	// Give orders taking priorities into account

	std::sort(orders.begin(), orders.end());
	for(const MyOrder& ord : orders)
	{
		switch (ord.ord.type)
		{
		case OrderType::movement:
			move(myUnits[ord.ord.unitId],ord.ord.dir);
			break;
		case OrderType::attack:
			attack(myUnits[ord.ord.unitId],ord.ord.dir);
			break;
		case OrderType::ability:
			ability(myUnits[ord.ord.unitId]);
			break;
		
		default:
			break;
		}
	}
} //play() end

/* To be called once */
void init()
{
	initOpeningMoves();
	initDirections();
	myUnits = units(me());
	commitedUnits = std::vector<int>(myUnits.size(),0);
	targets = Targets();
	targets.clear();
	orders = vector<MyOrder>(myUnits.size());
	for(int i = 0; i < orders.size(); ++i){
		orders[i].ord = Order(i,Direction::null,OrderType::movement);
	}
}

/* To be called once */
void initOpeningMoves()
{
	#define SR 27
	UNIT0OPENING = {
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
	UNIT1OPENING = {
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
	UNIT2OPENING = {
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
	UNIT3OPENING = {
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
	UNIT4OPENING = {
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
	UNIT5OPENING = {
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
	UNIT6OPENING = {
		Direction::UR,
		Direction::UR,
		Direction::up,
		Direction::up,
		Direction::up,
		Direction::up,
		Direction::up
	};
	UNIT7OPENING = {
		Direction::UR,
		Direction::UR,
		Direction::right,
		Direction::up,
		Direction::right
	};
}

/*
	Initializes DIRS_MAP
*/
void initDirections(){
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

/* Does opening moves iff the current round is less than OPENING_ROUNDS. Can be called in any round */
void doOpeningMoves()
{
	if(round() < OPENING_ROUNDS)
	{
			//Unit 1
		if(round() >= 3){
			if(round() == 3){
				fillSpecialDirections(Position(49,2),unit(myUnits[1]).position(),UNIT1SPECIAL[0],UNIT1SPECIAL[1]);
				move(myUnits[1],DIRS_MAP[UNIT1SPECIAL[0]]);
			}
			else if(round() == 4){  move(myUnits[1],DIRS_MAP[UNIT1SPECIAL[1]]);  }
			else{  move(myUnits[1],DIRS_MAP[UNIT1OPENING[round()-5]]);  }
		}

		//Unit 2
		if(round() >= 6){
			if(round() == 6){
				fillSpecialDirections(Position(47,1),unit(myUnits[2]).position(),UNIT2SPECIAL[0],UNIT2SPECIAL[1]);
				move(myUnits[2],DIRS_MAP[UNIT2SPECIAL[0]]);
			}
			else if(round() == 7){  move(myUnits[2],DIRS_MAP[UNIT2SPECIAL[1]]);  }
			else{  move(myUnits[2],DIRS_MAP[UNIT2OPENING[round()-8]]);  }
		}

		//Unit 3
		if(round() >= 9){
			if(round() == 9){
				fillSpecialDirections(Position(47,2),unit(myUnits[3]).position(),UNIT3SPECIAL[0],UNIT3SPECIAL[1]);
				move(myUnits[3],DIRS_MAP[UNIT3SPECIAL[0]]);
			}
			else if(round() == 10){  move(myUnits[3],DIRS_MAP[UNIT3SPECIAL[1]]);  }
			else{  move(myUnits[3],DIRS_MAP[UNIT3OPENING[round()-11]]);  }
		}

		//Unit 4
		if(round() >= 12){
			if(round() == 12){
				fillSpecialDirections(Position(47,0),unit(myUnits[4]).position(),UNIT4SPECIAL[0],UNIT4SPECIAL[1]);
				move(myUnits[4],DIRS_MAP[UNIT4SPECIAL[0]]);
			}
			else if(round() == 13){  move(myUnits[4],DIRS_MAP[UNIT4SPECIAL[1]]);  }
			else{  move(myUnits[4],DIRS_MAP[UNIT4OPENING[round()-14]]);  }
		}

		//Unit 5
		if(round() >= 15){
			if(round() == 15){
				fillSpecialDirections(Position(48,2),unit(myUnits[5]).position(),UNIT5SPECIAL[0],UNIT5SPECIAL[1]);
				move(myUnits[5],DIRS_MAP[UNIT5SPECIAL[0]]);
			}
			else if(round() == 16){  move(myUnits[5],DIRS_MAP[UNIT5SPECIAL[1]]);  }
			else{  move(myUnits[5],DIRS_MAP[UNIT5OPENING[round()-17]]);  }
		}

		//Unit 6
		if(round() >= 18){
			if(round() == 18){
				move(myUnits[6],DIRS_MAP[virtualPos(unit(myUnits[6]).position()).to(Position(48,1))]);
			}
			else if(round() == 19){
				//attack bubble
				Position aux = unit(myUnits[6]).position();
				Position targetPos = meleeTarget(aux,true,playerHasBubble);
				if(targetPos != Position(-1,-1)){
					attack(myUnits[6],aux.to(targetPos));
				}
			}
			else{  move(myUnits[6],DIRS_MAP[UNIT6OPENING[round()-20]]);  }
		}

		//Unit 7
		if(round() >= 21){
			Position aux = virtualPos(unit(myUnits[7]).position());
			if(aux.x < 47 or aux.y > 2){
				move(myUnits[7],DIRS_MAP[Direction::null]);
			}
			else{
				if(round() == 21){
					move(myUnits[7],DIRS_MAP[virtualPos(unit(myUnits[7]).position()).to(Position(48,1))]);
				}
				else{  move(myUnits[7],DIRS_MAP[UNIT7OPENING[round()-22]]); }
			}
		}

		//Unit 0
		move(myUnits[0],DIRS_MAP[UNIT0OPENING[round()]]);

		return;
	}
	else if(round() > OPENING_ROUNDS) return;

	//Else,
	//Very specific moves with a very specific order, don't touch
    if(virtualPos(unit(myUnits[8]).position()) == Position(47,2)){
      move(myUnits[8],DIRS_MAP[Direction::down]);
    }
    else if(virtualPos(unit(myUnits[9]).position()) == Position(47,2)){
      if(virtualPos(unit(myUnits[8]).position()) == Position(48,2)){
        move(myUnits[8],DIRS_MAP[Direction::down]);
      }
      move(myUnits[9],DIRS_MAP[Direction::down]);
    }
    move(myUnits[2],DIRS_MAP[Direction::down]);
    move(myUnits[0],DIRS_MAP[Direction::left]);
    move(myUnits[4],DIRS_MAP[Direction::down]);
    move(myUnits[1],DIRS_MAP[Direction::left]);
}

/* To be called every round */
void update()
{	
	updateUnits();
	targets.clear();
	orders = vector<MyOrder>(myUnits.size());
	for(int i = 0; i < orders.size(); ++i){
		orders[i].ord = Order(i,Direction::null,OrderType::movement);
	}
}

/* Used by updateUnits */
void updateUnits()
{
	//Update commitments and available units
	std::vector<int> newUnits = units(me());
	std::vector<int> newCommitedUnits(newUnits.size(),0);
	availableUnits = std::vector<bool>(newUnits.size(),true);

	for(int& value : commitedUnits)
		if(value > 0) --value;
	
	for(int i = 0; i < myUnits.size(); ++i)
	{
		if(commitedUnits[i] <= 0) continue;

		int index = dichSearch(myUnits[i],newUnits);
		if(index >= 0)
		{
			//Unit is there
			newCommitedUnits[index] = commitedUnits[i];
			availableUnits[index] = false;
		}
	}

	myUnits = newUnits;
	commitedUnits = newCommitedUnits;
}

/* Uses the ability if needed or useful */
void useAbility()
{
	//Currently uses the ability as soon as we get it
	for(int i = 0; i < myUnits.size(); ++i)
	{
		if(unit(myUnits[i]).upgraded())
		{
			orders[i] = MyOrder(Order(i,Direction::null,OrderType::ability),PRIO_ABILITY);
			availableUnits[i] = false;
			break;
		}
	}
}

/* Performs a BFS at the same time for all available units. This means no unit will explore distance 2 unless all units have explored distance 1 */
void layeredBFS(int initialRadius, int maxRadius)
{
	int remainingUnits = availableUnits.size();
	vector<bool> doneUnits = availableUnits;
	for(int i = 0; i < doneUnits.size(); ++i){
		doneUnits[i] = not doneUnits[i];
		if(doneUnits[i]) remainingUnits--;
	}

	vector<BFSInfo> layeredInfo(myUnits.size());
	for(int i = 0; i < layeredInfo.size(); ++i)
	{
		queueAdjacentPositions(BFSNode(0,unit(myUnits[i]).position()),layeredInfo[i].q,layeredInfo[i].v,(square(unit(myUnits[i]).position()).painter() == me()),me());
	}

	while(initialRadius < maxRadius and remainingUnits > 0)
	{
		for(int u = 0; u < myUnits.size(); ++u)
		{
			if(doneUnits[u]) continue;
			if(layerOfBFS(u,initialRadius,layeredInfo[u]))
			{
				doneUnits[u] = true;
				remainingUnits--;
			}
		}
		++initialRadius;
	}
}

/* Performs one layer of BFS (distance 'radius') for unit u.
   Gives orders if a bubble, bonus or unit is found.
*/
bool layerOfBFS(int u, int radius, BFSInfo& info)
{
	Position unitPos = unit(myUnits[u]).position();

	if(info.q.empty()) return false;
	bool diag = square(unit(myUnits[u]).position()).painter() == me();
	do{
		BFSNode n = info.q.front();
		if(n.dist > radius) return false;
		info.q.pop();

		Position pos = n.pos;
		Square sq = square(pos);

		queueAdjacentPositions(n,info.q,info.v,/*sq.painter() == me()*/false,me());

		//Evaluate conditions
		bool finished = false;

		if(sq.empty()) continue;
		if(targets.targets(pos) >= MAX_TARGETS) continue;
		if(sq.hasBubble())
		{
			Direction dir = bestDirectionSafe(pos,unit(myUnits[u]).position(),me(),diag);
			orders[u] = MyOrder(Order(u,dir,OrderType::movement),randomNumber(PRIO_FETCH,PRIO_FETCH+5));

			targets.target(pos);
			availableUnits[u] = false;
			finished = true;
		}
		else if(sq.hasBonus())
		{
			Direction dir = bestDirectionSafe(pos,unit(myUnits[u]).position(),me(),diag);
			orders[u] = MyOrder(Order(u,dir,OrderType::movement),randomNumber(PRIO_FETCH,PRIO_FETCH+5));

			targets.target(pos);
			availableUnits[u] = false;
			finished = true;
		}
		else if(sq.hasUnit() and sq.unit().player() != me() and (sq.painter() == me() or n.dist <= DIST_COMBAT))
		{
			Direction dir = bestDirectionSafe(pos,unit(myUnits[u]).position(),me(),diag);
			orders[u] = MyOrder(Order(u,dir,OrderType::movement),randomNumber(PRIO_FETCH,PRIO_FETCH+5));

			targets.target(pos);
			availableUnits[u] = false;
			finished = true;
		}

		if(finished)
		{
			return true;
		} 
	} while(not info.q.empty());
	return false;
}

/* Handles drawings: for units that are drawing, try to finish it; for units that are not, try to start a drawing. */
void doDrawingStuff()
{
	std::vector<int> available;
	for(int i = 0; i < availableUnits.size(); ++i)
		if(availableUnits[i])
			available.push_back(i);

	for(int u : available)
	{
		int un = myUnits[u];
		Square sq = square(unit(un).position());
		if(sq.drawn() and sq.unitDrawer() == un) continueDrawing(u);
		else startDrawing(u);
	}
	return;


	//Future, better implementation

	/*
	std::vector<int> commited;
	for(int i = 0; i < commitedUnits.size(); ++i)
		if(commitedUnits[i] > 0)
			commited.push_back(i);

	for(int u : commited)
	{
		//Continue drawing
		continueDrawing(u);
	}

	std::vector<int> available;
	for(int i = 0; i < availableUnits.size(); ++i)
		if(availableUnits[i])
			available.push_back(i);

	for(int u : available)
	{
		//Try to start a drawing
		startDrawing(u);
	}
	*/
}

void continueDrawing(int u)
{
	int un = myUnits[u];
	//If I'm drawing something, go to the nearest painted square
	Position p = unit(un).position();
	Square sq = square(p);

	if(not sq.drawn())
	{
		commitedUnits[u] = 0;
		availableUnits[u] = true;
		return;
	} 

	//BFS to nearest painted square
	Position pSqPos;
	std::function<bool(const Square&,int)> queueFunc = queueIfNotDrawn;
	std::function<bool(const Square&,int)> evalFunc = isMineSquare;
	if(findNearest(p,pSqPos,queueFunc,evalFunc))
	{
		vector<Direction> options = possibleDirections(pSqPos-p);
		vector<Direction> realOptions;

		for(int i = 0; i < options.size(); ++i)
		{
			Position paux = p+options[i];
			if(posOk(paux))
			{
				Square sqaux = square(paux);
        if(not sqaux.drawn() or sqaux.drawer() != me())
          realOptions.push_back(options[i]);
			}
		}

    Direction finalDir = Direction::null;

    if(realOptions.size() == 1)
    {
      finalDir = realOptions[0];
    }
    else if(realOptions.size() >= 2)
    {
      finalDir = realOptions[randomNumber(0,1)];
    }

		Order ord(u,finalDir,OrderType::movement);
		MyOrder order(ord,PRIO_MAX);
		orders[u] = order;	
	}	
}

void startDrawing(int u)
{
	Position p = unit(myUnits[u]).position();
	Position pSqPos;
	std::function<bool(const Square&,int)> queueFunc = queueIfNotDrawn;
	std::function<bool(const Square&,int)> evalFunc;

	Square sq = square(p);
	if(isMineSquare(sq,me()))
	{
		evalFunc = isNotMineSquare; 
	}
	else evalFunc = isMineSquare;

	if(findNearest(p,pSqPos,queueFunc,evalFunc))
	{
		Direction dir = Direction::null;
    if(isMineSquare(sq,me())) //Exit painted area
    {
      dir = bestDirectionFlex(pSqPos,p,isMineSquare(sq,me()));
      Position newPos = p+dir;
      if(posOk(newPos) and (not square(newPos).painted() or square(newPos).painter() != me()))
      {  
        dir = bestDirectionFlex(pSqPos,p,false);
      }
    }
    else //enter painted area avoiding drawings
    {
      vector<Direction> options = possibleDirections(pSqPos-p);
      vector<Direction> realOptions;
      for(int i = 0; i < options.size(); ++i)
      {
        Position paux = p+options[i];
        if(posOk(paux))
        {
          Square sqaux = square(paux);
          if(not sqaux.drawn() or sqaux.drawer() != me())
            realOptions.push_back(options[i]);
        }
      }

      if(realOptions.size() == 1)
      {
        dir = realOptions[0];
      }
      else if(realOptions.size() >= 2)
      {
        dir = realOptions[randomNumber(0,1)];
      }
    }
		Order ord(u,dir,OrderType::movement);
		MyOrder order(ord,PRIO_MAX);
		orders[u] = order;
	}
}

/*

	UTILITY

*/

template <typename T>
int dichSearch(const T& thing, const std::vector<T>& vec)
{
	return recursiveDichSearch(0,vec.size()-1,thing,vec);
}

template <typename T>
int recursiveDichSearch(int l, int r, const T& thing, const std::vector<T>& vec)
{
	if(r>l) return -1;
	int m = (l+r)/2;
	if(vec[m] > thing){
		return recursiveDichSearch(l,m-1,thing,vec);
	}
	else if(vec[m] < thing){
		return recursiveDichSearch(m+1,r,thing,vec);
	}
	return m;
}

static bool isDiagonal(Direction d){return d >= Direction::UL;}

/*
	Virtualizes a position as if you're down-left
*/
Position virtualPos(Position pos)
{
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

/*
	Specifically used in openings
*/
void fillSpecialDirections(Position desired, Position act, Direction& spec1, Direction& spec2){
  //Position is virtual
  //desired = virtualPos(desired);
  act = virtualPos(act);
  
  spec1 = bestDirection(desired,act);
  act +=spec1;
  spec2 = bestDirection(desired,act);
  act +=spec2;
}

/*
	Assumes diagonal moves are possible
*/
static Direction bestDirection(Position desired, Position act){
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

/* Returns all non-diagonal directions generated by pos, which is interpreted as a vector */
static vector<Direction> possibleDirections(Position pos)
{
	vector<Direction> dirs;

	if(pos.x < 0 and pos.y < 0)
	{
		dirs.push_back(Direction::up);
    dirs.push_back(Direction::left);
  }
  else if(pos.x > 0 and pos.y > 0)
	{
		dirs.push_back(Direction::down);
		dirs.push_back(Direction::right);
  }
  else if(pos.x == 0){
    if(pos.y < 0){
      dirs.push_back(Direction::left);
    }
    else dirs.push_back(Direction::right);
  }
  else if(pos.y == 0){
    if(pos.x < 0){
			dirs.push_back(Direction::up);
    }
		else dirs.push_back(Direction::down);
  }
  else if(pos.x < 0){
		dirs.push_back(Direction::up);
		dirs.push_back(Direction::right);
  }
  else if(pos.y < 0){
		dirs.push_back(Direction::down);
		dirs.push_back(Direction::left);
  }
  return dirs;
}

/* Returns one of the possible simple (non-diagonal) directions pos can generate if interpreted as a vector */
static Direction decide(Position pos){
	vector<Direction> possible = possibleDirections(pos);
	if(possible.size() >= 2)
	{
		if(randomNumber(0,1) == 0)
		{
			return possible[0];
		}
		return possible[1];
	}
	else if(possible.size() == 1)
		return possible[0];
	return Direction::null;  
}

/* Returns a direction to go from act to desired */
static Direction bestDirectionFlex(Position desired, Position act, bool diag = false){
  if(diag) return bestDirection(desired,act);
  return decide(Position(desired.x-act.x,desired.y-act.y));
}

static Direction bestDirectionSafe(Position desired, Position act, int col, bool diag = false)
{
	Direction result = bestDirectionFlex(desired,act,diag);
	
	if(isDiagonal(result))
	{
		//Check if exiting a painted area
		Position aux = act+result;
		if(posOk(aux))
		{
			Square sq = square(aux);
			if(sq.painter() != col)
				diag = false;
		}
	}

	if(not diag)
	{
		//Check if we step on drawings
		std::vector<Direction> dirs = possibleDirections(desired-act);
		std::vector<Direction> realDirs;

		for(Direction dir : dirs)
		{
			Position aux = act+dir;
			if(not posOk(aux)) continue;
			Square sq = square(aux);

			if(not sq.drawn() or sq.drawer() != col)
			{
				realDirs.push_back(dir);
			}
		}

		if(realDirs.empty())
			realDirs.push_back(Direction::null);

		result = realDirs[randomNumber(0,realDirs.size()-1)];
	}

	return result;
}

/*
	Returns a targeteable square in melee range according to the found function criteria. Used in openings
*/
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

/* Queues all valid adjacent positions in random order */
void queueAdjacentPositions(BFSNode info, queue<BFSNode>& toVisit, PositionSet& visited, bool tryDiagonal = false, int plId = -1)
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
    if(posOk(aux) and square(aux).drawer() != me() and not visited.queued(aux) and not visited.contains(aux))
	{
      if(isDiagonal(permutation[i]) and square(aux).painter() != plId) continue;
      toVisit.push(BFSNode(info.dist+1,aux));
      visited.queue(aux);
    }
  }
}

/* Uses BFS to find the nearest square sq such that evalFunc(sq)==true.
   Queues sq's adjacent positions if queueFunc(sq)==true.
   If it finds something, it returns true and the result is in result. Otherwise, it returns false. */
bool findNearest(Position ini, Position& result, std::function<bool(const Square&, int)> queueFunc, std::function<bool(const Square&,int)> evalFunc)
{
	BFSInfo info;
	BFSNode node(0,ini);
	queueAdjacentPositions(node,info.q,info.v,false,me());

	while(not info.q.empty())
	{
		BFSNode n = info.q.front();
		info.q.pop();
		if(not posOk(n.pos)) continue;
		if(n.dist > MAX_BFS) break;
		Square sq = square(n.pos);

		if(not queueFunc(sq,me())) continue;
		
		if(not evalFunc(sq,me()))
		{
			queueAdjacentPositions(n,info.q,info.v,false,me());
			continue;
		}

		result = n.pos;
		return true;
	}
	return false;
}

/*
	
	Utility "function pointers"

*/
static bool playerHasBubble(const Square& sq){ return sq.hasBubble(); }

static bool queueIfNotDrawn(const Square& sq, int me)
{
	return not sq.drawn() or sq.drawer() != me;
}

static bool isMineSquare(const Square& sq, int me)
{
	return sq.painted() and sq.painter() == me;
}

static bool isNotMineSquare(const Square& sq, int me)
{
	return not isMineSquare(sq,me);
}


}; //Struct end

REGISTER_PLAYER(PLAYER_NAME);