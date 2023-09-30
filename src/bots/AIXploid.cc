#include "../game/Player.hh"

#define PLAYER_NAME Xploid
using namespace std;

struct PLAYER_NAME : public Player{

static Player* factory(){ return new PLAYER_NAME; }

/*
	Opening moves
*/
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

vector<Direction> DIRS_MAP; //So directions work as if you were down-left
Position startingPos;

vector<int> myUnits;

void play()
{
	if(round() == 0)
	{
		init();
	}
} //play() end

void init()
{
	initOpeningMoves();
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
}; //Struct end