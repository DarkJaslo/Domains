#include "../game/Player.hh"
#include <functional>
#include <algorithm>

#define PLAYER_NAME Template
using namespace std;

struct PLAYER_NAME : public Player {

static Player* factory(){ return new PLAYER_NAME; }

/*

	CLASSES

*/

/* Ad hoc set that greatly improves BFS performance */
class PositionSet
{
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
    for (int i = 0; i < 50; ++i){
      for (int j = 0; j < 50; ++j){
        _container[Position(i,j)] = State::OUTSIDE;
      }
    }
  }
  void print(){
    std::cerr << "printing contents..." << std::endl;
    for (int i = 0; i < 50; ++i){
      for (int j = 0; j < 50; ++j){
        Position p(i,j);
        if (_container[p] != State::OUTSIDE){
          std::cerr << i << "," << j << " ";
        }
      }
    }
    std::cerr << std::endl;
  }
private:
  Matrix<int8_t> _container;  
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
vector<Direction> const DIRS_DIAGONAL = {Direction::UL, Direction::UR, Direction::DL, Direction::DR};
vector<Direction> const DIRS_STRAIGHT = {Direction::left, Direction::right, Direction::up, Direction::down};
vector<Direction> const DIRS_ALL = {Direction::left, Direction::right, Direction::up, Direction::down,
                                Direction::UL, Direction::UR, Direction::DL, Direction::DR};
vector<Direction> DIRS_MAP; //So directions work as if you were down-left

/*
    Global variables
*/
vector<int> MY_UNITS;

void play()
{
	if (round() == 0)
		Init();
	else 
		Update();

	if (round() <= OPENING_ROUNDS) 
		DoOpeningMoves();
	
} //play() end

/* To be called once */
void Init()
{
	InitOpeningMoves();
	InitDirections();
	MY_UNITS = units(me());
}

/* To be called once */
void InitOpeningMoves()
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
void InitDirections(){
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
      break;
    }
    default:
      break;
  }
}

/* Does opening moves iff the current round is less than OPENING_ROUNDS. Can be called in any round */
void DoOpeningMoves()
{
	if (round() < OPENING_ROUNDS)
	{
		// Unit 1
		if (round() >= 3){
			if (round() == 3){
				FillSpecialDirections(Position(49,2),unit(MY_UNITS[1]).position(),UNIT1SPECIAL[0],UNIT1SPECIAL[1]);
				move(MY_UNITS[1],DIRS_MAP[UNIT1SPECIAL[0]]);
			}
			else if (round() == 4){  move(MY_UNITS[1],DIRS_MAP[UNIT1SPECIAL[1]]);  }
			else{  move(MY_UNITS[1],DIRS_MAP[UNIT1OPENING[round()-5]]);  }
		}

		// Unit 2
		if (round() >= 6){
			if (round() == 6){
				FillSpecialDirections(Position(47,1),unit(MY_UNITS[2]).position(),UNIT2SPECIAL[0],UNIT2SPECIAL[1]);
				move(MY_UNITS[2],DIRS_MAP[UNIT2SPECIAL[0]]);
			}
			else if (round() == 7){  move(MY_UNITS[2],DIRS_MAP[UNIT2SPECIAL[1]]);  }
			else{  move(MY_UNITS[2],DIRS_MAP[UNIT2OPENING[round()-8]]);  }
		}

		// Unit 3
		if (round() >= 9){
			if (round() == 9){
				FillSpecialDirections(Position(47,2),unit(MY_UNITS[3]).position(),UNIT3SPECIAL[0],UNIT3SPECIAL[1]);
				move(MY_UNITS[3],DIRS_MAP[UNIT3SPECIAL[0]]);
			}
			else if (round() == 10){  move(MY_UNITS[3],DIRS_MAP[UNIT3SPECIAL[1]]);  }
			else{  move(MY_UNITS[3],DIRS_MAP[UNIT3OPENING[round()-11]]);  }
		}

		// Unit 4
		if (round() >= 12){
			if (round() == 12){
				FillSpecialDirections(Position(47,0),unit(MY_UNITS[4]).position(),UNIT4SPECIAL[0],UNIT4SPECIAL[1]);
				move(MY_UNITS[4],DIRS_MAP[UNIT4SPECIAL[0]]);
			}
			else if (round() == 13){  move(MY_UNITS[4],DIRS_MAP[UNIT4SPECIAL[1]]);  }
			else{  move(MY_UNITS[4],DIRS_MAP[UNIT4OPENING[round()-14]]);  }
		}

		// Unit 5
		if (round() >= 15){
			if (round() == 15){
				FillSpecialDirections(Position(48,2),unit(MY_UNITS[5]).position(),UNIT5SPECIAL[0],UNIT5SPECIAL[1]);
				move(MY_UNITS[5],DIRS_MAP[UNIT5SPECIAL[0]]);
			}
			else if (round() == 16){  move(MY_UNITS[5],DIRS_MAP[UNIT5SPECIAL[1]]);  }
			else{  move(MY_UNITS[5],DIRS_MAP[UNIT5OPENING[round()-17]]);  }
		}

		// Unit 6
		if (round() >= 18){
			if (round() == 18){
				move(MY_UNITS[6],DIRS_MAP[VirtualPos(unit(MY_UNITS[6]).position()).to(Position(48,1))]);
			}
			else if (round() == 19){
				//attack bubble
				Position aux = unit(MY_UNITS[6]).position();
				Position targetPos = MeleeTarget(aux,true,SquareHasBubble);
				if (targetPos != Position(-1,-1)){
					attack(MY_UNITS[6],aux.to(targetPos));
				}
			}
			else{  move(MY_UNITS[6],DIRS_MAP[UNIT6OPENING[round()-20]]);  }
		}

		// Unit 7
		if (round() >= 21){
			Position aux = VirtualPos(unit(MY_UNITS[7]).position());
			if (aux.x < 47 || aux.y > 2){
				move(MY_UNITS[7],DIRS_MAP[Direction::null]);
			}
			else{
				if (round() == 21){
					move(MY_UNITS[7],DIRS_MAP[VirtualPos(unit(MY_UNITS[7]).position()).to(Position(48,1))]);
				}
				else{  move(MY_UNITS[7],DIRS_MAP[UNIT7OPENING[round()-22]]); }
			}
		}

		// Unit 0
		move(MY_UNITS[0],DIRS_MAP[UNIT0OPENING[round()]]);

		return;
	}
	else if (round() > OPENING_ROUNDS) return;

	// Else,
	// Very specific moves with a very specific order, don't touch
    if (VirtualPos(unit(MY_UNITS[8]).position()) == Position(47,2)){
      move(MY_UNITS[8],DIRS_MAP[Direction::down]);
    }
    else if (VirtualPos(unit(MY_UNITS[9]).position()) == Position(47,2)){
      if (VirtualPos(unit(MY_UNITS[8]).position()) == Position(48,2)){
        move(MY_UNITS[8],DIRS_MAP[Direction::down]);
      }
      move(MY_UNITS[9],DIRS_MAP[Direction::down]);
    }
    move(MY_UNITS[2],DIRS_MAP[Direction::down]);
    move(MY_UNITS[0],DIRS_MAP[Direction::left]);
    move(MY_UNITS[4],DIRS_MAP[Direction::down]);
    move(MY_UNITS[1],DIRS_MAP[Direction::left]);
}

/* To be called every round */
void Update()
{	
	UpdateUnits();
}

/* Used by update */
void UpdateUnits()
{
	MY_UNITS = units(me());
}

/*
	UTILITY
*/

template <typename T>
int FindInVector(T const& thing, std::vector<T> const& vec)
{
	return RecursiveFind(0,vec.size()-1,thing,vec);
}

template <typename T>
int RecursiveFind(int l, int r, T const& thing, std::vector<T> const& vec)
{
	if (r>l) 
		return -1;

	int m = (l+r)/2;

	if (vec[m] > thing)
		return RecursiveFind(l,m-1,thing,vec);
	else if (vec[m] < thing)
		return RecursiveFind(m+1,r,thing,vec);

	return m;
}

static bool IsDiagonal(Direction d){return d >= Direction::UL;}

/*
	Virtualizes a position as if you're down-left
*/
Position VirtualPos(Position pos)
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
	Specifically used in openings, leave as-is
*/
void FillSpecialDirections(Position desired, Position act, Direction& spec1, Direction& spec2){
  // The desired position is already virtual
  // desired = VirtualPos(desired);
  act = VirtualPos(act);
  
  spec1 = BestDirection(desired,act);
  act += spec1;
  spec2 = BestDirection(desired,act);
  act += spec2;
}

/*
	Assumes diagonal moves are possible
*/
static Direction BestDirection(Position desired, Position act){
  if (act == desired) return Direction::null;
  else if (desired.x > act.x && desired.y > act.y){return Direction::DR;}
  else if (desired.x < act.x && desired.y < act.y){return Direction::UL;}
  else if (desired.x > act.x && desired.y < act.y){return Direction::DL;}
  else if (desired.x < act.x && desired.y > act.y){return Direction::UR;}
  else if (desired.x > act.x){ return Direction::down; }
  else if (desired.x < act.x){ return Direction::up; }
  else if (desired.y > act.y){ return Direction::right; }
  return Direction::left;
}

/* Returns all non-diagonal directions generated by pos, which is interpreted as a vector */
static vector<Direction> PossibleStraightDirections(Position pos)
{
	vector<Direction> dirs;

	if (pos.x < 0 && pos.y < 0)
	{
		dirs.push_back(Direction::up);
		dirs.push_back(Direction::left);
	}
	else if (pos.x > 0 && pos.y > 0)
	{
		dirs.push_back(Direction::down);
		dirs.push_back(Direction::right);
	}
	else if (pos.x == 0)
	{
		if (pos.y < 0)
			dirs.push_back(Direction::left);
		else 
			dirs.push_back(Direction::right);
	}
	else if (pos.y == 0)
	{
		if (pos.x < 0)
			dirs.push_back(Direction::up);
		else 
			dirs.push_back(Direction::down);
	}
	else if (pos.x < 0)
	{
		dirs.push_back(Direction::up);
		dirs.push_back(Direction::right);
	}
	else if (pos.y < 0)
	{
		dirs.push_back(Direction::down);
		dirs.push_back(Direction::left);
	}
	return dirs;
}

/* Returns one of the possible simple (non-diagonal) directions pos can generate if interpreted as a vector */
static Direction OneStraightDirection(Position pos){
	vector<Direction> possible = PossibleStraightDirections(pos);
	if (possible.size() >= 2)
	{
		if (randomNumber(0,1) == 0)
			return possible[0];

		return possible[1];
	}
	else if (possible.size() == 1)
	{
		return possible[0];
	}

	return Direction::null;  
}

/* Returns a direction to go from act to desired. If diag=true, also considers diagonal moves */
static Direction BestDirectionFlex(Position desired, Position act, bool diag = false)
{
  	if (diag) 
  		return BestDirection(desired,act);

  	return OneStraightDirection(Position(desired.x-act.x,desired.y-act.y));
}

static Direction BestDirectionSafe(Position desired, Position act, int col, bool diag = false)
{
	Direction result = BestDirectionFlex(desired,act,diag);
	
	if (IsDiagonal(result))
	{
		//Check if exiting a painted area
		Position aux = act+result;
		if (posOk(aux))
		{
			Square sq = square(aux);
			if (sq.painter() != col)
				diag = false;
		}
	}

	if (!diag)
	{
		//Check if we step on drawings
		std::vector<Direction> dirs = PossibleStraightDirections(desired-act);
		std::vector<Direction> realDirs;

		for (Direction dir : dirs)
		{
			Position aux = act+dir;
			if (!posOk(aux)) continue;
			Square sq = square(aux);

			if (!sq.drawn() || sq.drawer() != col)
			{
				realDirs.push_back(dir);
			}
		}

		if (realDirs.empty())
			realDirs.push_back(Direction::null);

		result = realDirs[randomNumber(0,realDirs.size()-1)];
	}

	return result;
}

/*
	Returns a targeteable square in melee range according to the found function criteria. Used in openings
*/
Position MeleeTarget(Position act, bool diag, bool(*found)(const Square&))
{
  vector<Direction> dirs;
  if (diag) dirs = DIRS_ALL;
  else dirs = DIRS_STRAIGHT;

  for (int i = 0; i < dirs.size(); ++i){
    Position aux = act+dirs[i];
    if (!posOk(aux)) continue;
    Square sq = square(aux);
    if (found(sq)){
      return aux;
    }
  }

  return Position(-1,-1);
}

/*
	Utility "function pointers"
*/
static bool SquareHasBubble(Square const& sq){ return sq.hasBubble(); }

}; //Struct end

REGISTER_PLAYER(PLAYER_NAME);