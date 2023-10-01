#include "../game/Player.hh"

#define PLAYER_NAME Xploid
using namespace std;

struct PLAYER_NAME : public Player{

static Player* factory(){ return new PLAYER_NAME; }

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
Position startingPos;

vector<int> myUnits;

void play()
{
	if(round() == 0)
	{
		init();
	}

	if(round() < OPENING_ROUNDS)
	{
		doOpeningMoves();
		return;
	}
	//Else

	/*
	1.Handle critical stuff
	
	To remaining, free units:

	2.Layered bfs of depth D, choose
		-Bubble
		-Bonus
		-Combat in my domain
		
	To remaining units:

	3.Try to draw (commitment, not free anymore)

	
	
	*/


} //play() end

void init()
{
	initOpeningMoves();
	initDirections();
}

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

void doOpeningMoves()
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
}








/*

	UTILITY

*/

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

/*
	Returns a targeteable square in melee range according to the found function criteria
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



/*
	
	Utility "function pointers"

*/
static bool playerHasBubble(const Square& sq){ return sq.hasBubble(); }
//static bool playerHasBubble(const BFSStruct& str){ return str.sq.hasBubble(); }

}; //Struct end