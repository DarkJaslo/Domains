#include "../game/Player.hh"
#include <functional>
#include <algorithm>

#define PLAYER_NAME Template
using namespace std;

/*
	CONFIGURATION
*/
bool const ENABLE_OPENING { true };
bool const ENABLE_PRIORITIES { true };
bool const ENABLE_FIGHTS { true };
bool const ENABLE_ATTACKS { true };
bool const ENABLE_BUBBLE_POPS { true };
bool const ENABLE_BONUS_COLLECTION { true };
bool const ENABLE_BAD_DIAG_HANDLE { true }; // Enemy has unfair fight position against you
bool const ENABLE_STEP_ON_DRAWING { true };

enum class AbilityPolicy
{ 
	IMMEDIATE // Use try to use it as soon as you have it 
};
AbilityPolicy ABILITY_POLICY { AbilityPolicy::IMMEDIATE };

int const MAX_TARGETS {1};
int const MAX_CRITICAL_BFS {100};


int const PRIO_AVOID_UNFAIR { 120 };
int const PRIO_ATTACK_1 { 100 };
int const PRIO_USE_ABILITY { 90 };
int const PRIO_ATTACK_BUBBLE { 10 };
int const PRIO_GET_CLOSE { 0 };


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
  PositionSet()
	: _container{50, 50}
  {
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
	for (auto&& p : _container)
		p = State::OUTSIDE;
  }
  void print(){
    std::cerr << "printing contents..." << std::endl;
    for (int i = 0; i < rows(); ++i){
      for (int j = 0; j < cols(); ++j){
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

class Targets
{
public:
	Targets(int max_targets)
		: m_max_targets{max_targets}, m_board{50, 50}
	{ 
		clear(); 
	}

	void clear()
	{
		for (auto&& targets : m_board)
			targets = 0;
	}

	bool can(Position p) const
	{
		return m_board[p] < m_max_targets;
	}

	unsigned int targets(Position p) const
	{
		return m_board[p];
	}

	void add(Position p)
	{
		++m_board[p];
	}

	void remove(Position p)
	{
		if (targets(p) > 0)
			--m_board[p];
	}
private:
	int m_max_targets;
	Matrix<std::uint16_t> m_board;
};

class LayeredBFS
{
public:
	/*
		max_depth is the maximum distance or depth that will be visited

		sources is a list of source positions

		visitor is called for each visited Square on source i and at distance d

		on_depth_completion is called for each source index when all sources have 
		visited all Squares at each distance/depth. This must return true if the user 
		wants to stop the search from a specific source from now on

		queuer returns all the directions from the Square passed as parameter that
		the user wants to enqueue
	*/
	LayeredBFS(int max_depth,
			   std::vector<Position> const& sources,
			   std::function<void(Square const&, int, int)> visitor,
			   std::function<bool(int)> on_depth_completion,
			   std::function<std::vector<Direction>(Square const&)> queuer)
		: m_max_depth(max_depth), m_sources(sources.size()), m_visitor(visitor), 
		  m_on_depth_completion(on_depth_completion), m_queuer(queuer),
		  m_searches{sources.size()}, m_queues{sources.size()}
	{
		m_finished.resize(sources.size());
		
		for (int i {0}; i < sources.size(); ++i)
		{
			m_queues[i].emplace(sources[i], 0);
			m_searches[i].queue(sources[i]);
		}
	}

	void run()
	{
		while (!finished())
		{
			for (int source {0}; source < m_sources; ++source)
			{
				if (m_finished[source])
					continue;

				auto& queue = m_queues[source];
				auto& search = m_searches[source];
				while (!queue.empty())
				{
					if (std::get<int>(queue.front()) > m_current_depth)
						break;

					auto pos = std::get<Position>(queue.front());
					queue.pop();

					auto const& sq = square(pos);
					auto directions = m_queuer(sq);
					for (auto&& dir : directions)
					{
						auto new_pos {pos + dir};
						if (!posOk(new_pos))
							continue;

						if (search.queued(new_pos) || search.contains(new_pos))
							continue;

						queue.emplace(new_pos, m_current_depth+1);
						search.queue(new_pos);
					}

					m_visitor(sq, source, m_current_depth);
					search.add(pos);
				}
			}

			for (int source {0}; source < m_sources; ++source)
				if (m_on_depth_completion(source))
					m_finished[source] = true;

			++m_current_depth;
		}
	}

private:
	bool finished() const
	{
		if (m_current_depth > m_max_depth)
			return true;

		for (auto&& b : m_finished)
			if (!b)
				return false;

		return true;
	}

	int m_max_depth;
	int m_sources;
	std::function<void(Square const&, int, int)> m_visitor;
	std::function<bool(int)> m_on_depth_completion;
	std::function<std::vector<Direction>(Square const&)> m_queuer;

	// State
	int m_current_depth{0};
	std::vector<bool> m_finished;
	std::vector<PositionSet> m_searches;
	std::vector<std::queue<std::tuple<Position, int>>> m_queues;
};

struct MyOrder
{
	Order order;
	int priority;
	Position target_pos;

	bool operator<(MyOrder const& other) const 
	{
		return this->priority > other.priority;
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
vector<Direction> const DIRS_DIAGONAL = {Direction::UL, Direction::UR, Direction::DL, Direction::DR};
vector<Direction> const DIRS_STRAIGHT = {Direction::left, Direction::right, Direction::up, Direction::down};
vector<Direction> const DIRS_ALL = {Direction::UL, Direction::UR, Direction::DL, Direction::DR, Direction::left, Direction::right, Direction::up, Direction::down};
vector<Direction> DIRS_MAP; //So directions work as if you were down-left

/*
    Global variables
*/
vector<int> MY_UNITS;
vector<MyOrder> ORDERS;
vector<int> BUSY;
Targets TARGETS{MAX_TARGETS};

void play()
{
	if (round() == 0)
		Init();
	else 
		Update();

	if (ENABLE_OPENING && round() <= OPENING_ROUNDS) 
	{
		DoOpeningMoves();
		return;
	}

	HandleCriticalCases();

	HandleAbility();

	AssignUnitsToConsumables();

	ManageDrawings();

	SendOrders();	
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
	int rnd = round();

	if (rnd < OPENING_ROUNDS)
	{
		// Unit 1
		if (rnd >= 3){
			if (rnd == 3){
				FillSpecialDirections(Position(49,2),unit(MY_UNITS[1]).position(),UNIT1SPECIAL[0],UNIT1SPECIAL[1]);
				move(MY_UNITS[1],DIRS_MAP[UNIT1SPECIAL[0]]);
			}
			else if (rnd == 4){  move(MY_UNITS[1],DIRS_MAP[UNIT1SPECIAL[1]]);  }
			else{  move(MY_UNITS[1],DIRS_MAP[UNIT1OPENING[rnd-5]]);  }
		}

		// Unit 2
		if (rnd >= 6){
			if (rnd == 6){
				FillSpecialDirections(Position(47,1),unit(MY_UNITS[2]).position(),UNIT2SPECIAL[0],UNIT2SPECIAL[1]);
				move(MY_UNITS[2],DIRS_MAP[UNIT2SPECIAL[0]]);
			}
			else if (rnd == 7){  move(MY_UNITS[2],DIRS_MAP[UNIT2SPECIAL[1]]);  }
			else{  move(MY_UNITS[2],DIRS_MAP[UNIT2OPENING[rnd-8]]);  }
		}

		// Unit 3
		if (rnd >= 9){
			if (rnd == 9){
				FillSpecialDirections(Position(47,2),unit(MY_UNITS[3]).position(),UNIT3SPECIAL[0],UNIT3SPECIAL[1]);
				move(MY_UNITS[3],DIRS_MAP[UNIT3SPECIAL[0]]);
			}
			else if (rnd == 10){  move(MY_UNITS[3],DIRS_MAP[UNIT3SPECIAL[1]]);  }
			else{  move(MY_UNITS[3],DIRS_MAP[UNIT3OPENING[rnd-11]]);  }
		}

		// Unit 4
		if (rnd >= 12){
			if (rnd == 12){
				FillSpecialDirections(Position(47,0),unit(MY_UNITS[4]).position(),UNIT4SPECIAL[0],UNIT4SPECIAL[1]);
				move(MY_UNITS[4],DIRS_MAP[UNIT4SPECIAL[0]]);
			}
			else if (rnd == 13){  move(MY_UNITS[4],DIRS_MAP[UNIT4SPECIAL[1]]);  }
			else{  move(MY_UNITS[4],DIRS_MAP[UNIT4OPENING[rnd-14]]);  }
		}

		// Unit 5
		if (rnd >= 15){
			if (rnd == 15){
				FillSpecialDirections(Position(48,2),unit(MY_UNITS[5]).position(),UNIT5SPECIAL[0],UNIT5SPECIAL[1]);
				move(MY_UNITS[5],DIRS_MAP[UNIT5SPECIAL[0]]);
			}
			else if (rnd == 16){  move(MY_UNITS[5],DIRS_MAP[UNIT5SPECIAL[1]]);  }
			else{  move(MY_UNITS[5],DIRS_MAP[UNIT5OPENING[rnd-17]]);  }
		}

		// Unit 6
		if (rnd >= 18){
			if (rnd == 18){
				move(MY_UNITS[6],DIRS_MAP[VirtualPos(unit(MY_UNITS[6]).position()).to(Position(48,1))]);
			}
			else if (rnd == 19){
				//attack bubble
				Position aux = unit(MY_UNITS[6]).position();
				Position targetPos = MeleeTarget(aux,true,SquareHasBubble);
				if (targetPos != Position(-1,-1)){
					attack(MY_UNITS[6],aux.to(targetPos));
				}
			}
			else{  move(MY_UNITS[6],DIRS_MAP[UNIT6OPENING[rnd-20]]);  }
		}

		// Unit 7
		if (rnd >= 21){
			Position aux = VirtualPos(unit(MY_UNITS[7]).position());
			if (aux.x < 47 || aux.y > 2){
				move(MY_UNITS[7],DIRS_MAP[Direction::null]);
			}
			else{
				if (rnd == 21){
					move(MY_UNITS[7],DIRS_MAP[VirtualPos(unit(MY_UNITS[7]).position()).to(Position(48,1))]);
				}
				else{  move(MY_UNITS[7],DIRS_MAP[UNIT7OPENING[rnd-22]]); }
			}
		}

		// Unit 0
		move(MY_UNITS[0],DIRS_MAP[UNIT0OPENING[rnd]]);

		return;
	}
	else if (rnd > OPENING_ROUNDS) return;

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
	ORDERS.clear();
	TARGETS.clear();
}

/* Used by update */
void UpdateUnits()
{
	MY_UNITS = units(me());
	if (MY_UNITS.back() >= BUSY.size())
		BUSY.resize(MY_UNITS.back()+1);

	for (auto&& x : BUSY)
		x = false;
}

void HandleCriticalCases()
{
	/*
	Layered BFS for a limited radius for all units that aren't doing anything
	
	We will check all squares at the same distance first, then evaluate options:
	- if we encounter a case where we *could* do something, add the thing we would do to an
	  order list. Then when we have checked all squares at a certain distance we check that
	  order list and:
	  	- if there is nothing, keep bfsing
		- if there is something, select the most prioritary option. If there is a tie get 
		  the first one
	*/

	std::vector<Position> sources;
	std::vector<int> index_map;

	for (int i {0}; i < MY_UNITS.size(); ++i)
		if (!BUSY[MY_UNITS[i]])
		{
			sources.push_back(unit(MY_UNITS[i]).position());
			index_map.push_back(MY_UNITS[i]);
		}

	std::vector<std::vector<MyOrder>> options;
	options.resize(sources.size());

	LayeredBFS bfs
	{
		MAX_CRITICAL_BFS,
		sources,
		// Visitor
		[&options, &sources, &index_map, me = me(), this](Square const& sq, int index, int distance)
		{
			auto& my_options = options[index];
			auto& source = sources[index];
			/*
			Distance 1:
			If there is a fight, decide accordingly
			If there is a bubble pop it
			If there is a drawing step on it
			If there is a bonus go for it

			Any other distance:
			Same, but on the fight case move instead of attacking
			*/
			auto GetClose = [&source, &sq, me, this](MyOrder& option)
			{
				option.order.type = OrderType::movement;
				option.order.dir = source.to(sq.pos());
				option.priority = PRIO_GET_CLOSE;
				option.target_pos = sq.pos();

				if (utils::isDiagonal(option.order.dir) && (!sq.painted() || sq.painter() != me))
				{
					auto [dir_x, dir_y] = utils::decompose(option.order.dir);

					if (randomNumber(0,1) == 0)
						option.order.dir = dir_x;
					else
						option.order.dir = dir_y;
				}
			};

			if (ENABLE_FIGHTS && sq.hasUnit() && sq.unit().player() != me)
			{
				// Target this unit
				MyOrder option;

				if (distance == 1)
				{
					option.order.type = ENABLE_ATTACKS ? OrderType::attack : OrderType::movement;
					option.order.dir = source.to(sq.pos());
					option.priority = PRIO_ATTACK_1;
					option.target_pos = sq.pos();
					// Don't need to set unitId here
				}
				else if (distance == 2)
				{
					// Check enemy advantage - are they on their square?
					if (sq.painted() && sq.painter() == sq.unit().player())
					{
						Position vec { sq.pos() - source };
						// Check if they can unfairly target us
						if (abs(vec.x) > 0 && abs(vec.y) > 0 && abs(vec.x) + abs(vec.y) == 2)
						{
							option.priority = PRIO_AVOID_UNFAIR;
							option.order.type = OrderType::movement;

							if (ENABLE_BAD_DIAG_HANDLE)
							{
								// Run away
								auto [dir_x, dir_y] = utils::decompose(sq.pos().to(source));
								if (randomNumber(0,1) == 0)
									option.order.dir = dir_x;
								else
									option.order.dir = dir_y;
							}
							else
							{
								// Dumber -> Get close
								auto [dir_x, dir_y] = utils::decompose(source.to(sq.pos()));
								if (randomNumber(0,1) == 0)
									option.order.dir = dir_x;
								else
									option.order.dir = dir_y;
							}

							option.target_pos = source + option.order.dir;
						}
					}
					else
					{
						// Just get close for now
						GetClose(option);
					}
				}
				else
				{
					// Just get close for now
					GetClose(option);
				}

				if (TARGETS.can(option.target_pos))
				{
					my_options.push_back(option);
				}
			}

			if (ENABLE_BUBBLE_POPS && sq.hasBubble())
			{
				MyOrder option;

				if (distance == 1)
				{
					// Attack it
					option.order.type = OrderType::attack;
					option.order.dir = source.to(sq.pos());
					option.priority = PRIO_ATTACK_BUBBLE;
					option.target_pos = sq.pos();
				}
				else
				{
					GetClose(option);
				}

				if (TARGETS.can(option.target_pos))
				{
					my_options.push_back(option);
				}
			}
		
			if (ENABLE_STEP_ON_DRAWING)
			{

			}

			if (ENABLE_BONUS_COLLECTION && sq.hasBonus())
			{
				MyOrder option;
				GetClose(option);

				if (TARGETS.can(option.target_pos))
				{
					my_options.push_back(option);
				}
			}
		
			if (unit(MY_UNITS[index]).upgraded())
			{
				if (ABILITY_POLICY == AbilityPolicy::IMMEDIATE)
				{
					MyOrder option;
					option.order.dir = Direction::null;
					option.order.type = OrderType::ability;
					option.priority = PRIO_USE_ABILITY;
					option.target_pos = source;

					my_options.push_back(option);
				}
			}
		},
		// On depth completion
		[&options, &index_map, &sources, this](int index) -> bool
		{
			auto& my_options = options[index];
			if (my_options.empty())
			{
				return false;
			}

			std::sort(my_options.begin(), my_options.end());

			for (int option_index {0}; option_index < my_options.size(); ++option_index)
			{
				MyOrder option = my_options[option_index];

				if (option.order.type == OrderType::movement || option.order.type == OrderType::attack)
				{
					Position p {sources[index]+option.order.dir};


					if (!posOk(p) || !TARGETS.can(p) || !TARGETS.can(option.target_pos))
					{
						continue;
					}
					else
					{
						TARGETS.add(p);
						if (TARGETS.can(option.target_pos))
							TARGETS.add(option.target_pos);
					}
				}
				else
				{
					if (!TARGETS.can(sources[index]))
						continue;
					else 
						TARGETS.add(sources[index]);
				}

				// Translate index
				int unit_index = index_map[index];
				option.order.unitId = index_map[index];
				ORDERS.push_back(option);
				BUSY[unit_index] = true;

				my_options.clear();
				return true;
			}

			return false;
		},
		// Queuer
		[me = me(), this](Square const& sq) -> std::vector<Direction>
		{
			std::vector<Direction> dirs;

			if (sq.painter() == me)
			{
				for (auto&& dir : DIRS_DIAGONAL)
				{
					Position p { sq.pos() + dir };
					if (posOk(p))
						dirs.push_back(dir);
				}
			}

			for (auto&& dir : DIRS_STRAIGHT)
			{
				Position p { sq.pos() + dir };
				if (posOk(p))
					dirs.push_back(dir);
			}
			
			return dirs;
		}
	};
	bfs.run();

}

void HandleAbility()
{
	/*
	If there is an upgraded unit, decide what to do with it:
		- Maybe it's already doing something, in that case skip
		- If not, decide an action depending on the configuration
	*/
}

void AssignUnitsToConsumables()
{
	/*
	LayeredBFS from consumables to free players, mainly because this way we can pathfind 
	using the diagonal movement intelligently

	If that does not work for some reason, just do some vector math
	*/
}

void ManageDrawings()
{
	/*
	Start, continue and finish drawings
	*/
}

void SendOrders()
{
	if (!ENABLE_PRIORITIES)
	{
		// Assign random values to each order before sorting
		for (auto&& ord : ORDERS)
			ord.priority = randomNumber(0, 200);
	}

	std::sort(ORDERS.begin(), ORDERS.end());

	for (auto&& ord : ORDERS)
	{
		switch (ord.order.type)
		{
		case OrderType::movement:
			move(ord.order.unitId, ord.order.dir);
			break;
		case OrderType::attack:
			attack(ord.order.unitId, ord.order.dir);
			break;
		case OrderType::ability:
			ability(ord.order.unitId);
			break;
		}
	}
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