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
bool const ENABLE_BONUS_COLLECTION { false };
bool const ENABLE_BAD_DIAG_HANDLE { true }; // Enemy has unfair fight position against you
bool const ENABLE_STEP_ON_DRAWING { false }; // Makes player worse if true
bool const ENABLE_DRAWING { true };
bool const ENABLE_SAFE_APPROACH { true }; // At distance 2 and enemy distance 3 get close without using diagonals
bool const ENABLE_DISTANCE2_INTELLIGENCE { true };
bool const ENABLE_DOMAIN_EXPANSION { true };
bool const ENABLE_USEFUL_ABILITIES { true };
bool const ENABLE_REALLY_USEFUL_ABILITIES { true };
bool const ENABLE_FILLER { true }; // Tries to draw on tight sections, like bottlenecks

int const MAX_TARGETS {1};
int const MAX_CRITICAL_BFS {2};
int const MAX_DRAW_BFS {100};
int const MAX_WAIT_ROUNDS {1}; // 0 means no waiting
int const MAX_CONSUMABLE_BFS { 40 };
int const MAX_CONSUMABLE_DIST_DIFF { 2 };
int const MAX_FILLER_CHECK { 10 };
double const MIN_FILLER_RATIO { 4.5 };

int const DRAW_AWAY_DIST {2};
int const DRAW_SIDE_DIST {2};

int const PRIO_AVOID_UNFAIR { 120 };
int const PRIO_ATTACK_1 { 100 };
int const PRIO_USE_ABILITY { 90 };
int const PRIO_ATTACK_BUBBLE { 10 };
int const PRIO_STEP_ON_DRAWING { 9 };
int const PRIO_SEARCH { 5 };
int const PRIO_GET_CLOSE { 0 };
int const PRIO_DRAW { 0 };
int const PRIO_ATTACK_WAIT { 99 };

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

	struct Info
	{
		Position pos;
		Direction from_where;
		int distance;
	};
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
			   std::function<void(Square const&, Direction, int, int)> visitor,
			   std::function<bool(int)> on_depth_completion,
			   std::function<std::vector<Direction>(Square const&)> queuer)
		: m_max_depth(max_depth), m_sources(sources.size()), m_visitor(visitor), 
		  m_on_depth_completion(on_depth_completion), m_queuer(queuer),
		  m_searches{sources.size()}, m_queues{sources.size()}
	{
		m_finished.resize(sources.size());
		
		for (int i {0}; i < sources.size(); ++i)
		{
			m_queues[i].push(Info{sources[i], Direction::null, 0});
			m_searches[i].queue(sources[i]);
		}
	}

	void run()
	{
		while (!finished())
			run_layer();
	}

	bool run_layer()
	{
		for (int source {0}; source < m_sources; ++source)
		{
			if (m_finished[source])
				continue;

			auto& queue = m_queues[source];
			auto& search = m_searches[source];
			while (!queue.empty())
			{
				if (queue.front().distance > m_current_depth)
					break;

				auto info = queue.front();
				queue.pop();

				auto const& sq = square(info.pos);
				auto directions = m_queuer(sq);
				for (auto&& dir : directions)
				{
					auto new_pos {info.pos + dir};
					if (!posOk(new_pos))
						continue;

					if (search.queued(new_pos) || search.contains(new_pos))
						continue;

					Direction first_move = info.from_where; 
					if (first_move == Direction::null)
						first_move = dir;

					queue.push(Info{new_pos, first_move, m_current_depth+1});
					search.queue(new_pos);
				}

				m_visitor(sq, info.from_where, source, m_current_depth);
				search.add(info.pos);
			}
		}

		for (int source {0}; source < m_sources; ++source)
			if (m_on_depth_completion(source))
				m_finished[source] = true;

		++m_current_depth;
		return m_current_depth > m_max_depth;
	}

	bool finished() const
	{
		if (m_current_depth > m_max_depth)
			return true;

		for (auto&& b : m_finished)
			if (!b)
				return false;

		return true;
	}

private:
	int m_max_depth;
	int m_sources;
	std::function<void(Square const&, Direction, int, int)> m_visitor;
	std::function<bool(int)> m_on_depth_completion;
	std::function<std::vector<Direction>(Square const&)> m_queuer;

	// State
	int m_current_depth{0};
	std::vector<bool> m_finished;
	std::vector<PositionSet> m_searches;
	std::vector<std::queue<Info>> m_queues;
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

class Distancer
{
public:
	Distancer(int max_distance,
			  Position const& source,
			  std::function<std::vector<Direction>(Square const&)> queuer)
		: m_distances { 50, 50 }
	{
		std::vector sources { source };
		for (auto&& x : m_distances)
			x = -1;

		m_bfs = new LayeredBFS
		{
			max_distance,
			sources,
			// Visitor
			[this](Square const& sq, Direction first_dir, int index, int distance)
			{
				m_distances[sq.pos()] = distance;
			},
			// Depth completion
			[](int index) -> bool
			{
				return false;
			},
			queuer
		};
	}

	~Distancer()
	{
		delete m_bfs;
	}

	// -1 means "not found"
	int Distance(Position target)
	{
		while (!m_bfs->finished() && m_distances[target] == -1)
			m_bfs->run_layer();

		return m_distances[target];
	}

private:
	LayeredBFS* m_bfs;
	Matrix<int16_t> m_distances;
};

enum class DrawingState
{
	Away,
	Side,
	Return,
	None
};

class Drawing
{
public:
	Drawing() = default;
	Drawing(PLAYER_NAME* player, int unit) : m_player{player}, m_unit{unit} {}

	void Process()
	{
		/*
		Start, continue and finish drawings

		For all free units
		If it's drawing, still drawing? (finished a drawing midway or drawing got erased)
			Getting away:
				finished: choose a perpendicular direction and move, transition to side
			Side:
				finished: go to nearest painted area

		If it's not drawing
			If inside painted area:
				if it can exit with one straight move, do so
				if not, find nearest unpainted square

			If outside painted area: go to painted area
		
		*/

		Position act { unit(m_unit).position() };

		if (IsDrawing())
		{
			auto AwayToSide = [this]()
			{
				m_state = DrawingState::Side;
				auto [dirA, dirB] = utils::perpendiculars(m_away_dir);
				m_side_dir = randomNumber(0, 1) == 0 ? dirA : dirB;
				m_side_steps = 0;
			};

			switch (m_state)
			{
			case DrawingState::Away:
			{
				Position next { act + m_away_dir };
				if (!posOk(next) || (square(next).drawn() && square(next).drawer() == m_player->me()))
				{ 
					AwayToSide();
				}
				else
				{
					MyOrder order {};
					order.target_pos = next;
					order.priority = PRIO_DRAW;
					order.order.dir = m_away_dir;
					order.order.type = OrderType::movement;
					order.order.unitId = m_unit;
					m_player->ORDERS.push_back(order);

					if (++m_away_steps >= DRAW_AWAY_DIST)
						AwayToSide();
				}
				break;
			}
			case DrawingState::Side:
			{
				if (ENABLE_FILLER)
				{
					if (TryFiller(act))
						return;
				}

				Position next { act + m_side_dir };
				if (!posOk(next) || (square(next).drawn() && square(next).drawer() == m_player->me()))
					m_state = DrawingState::Return;
				else
				{
					MyOrder order {};
					order.target_pos = next;
					order.priority = PRIO_DRAW;
					order.order.dir = m_side_dir;
					order.order.type = OrderType::movement;
					order.order.unitId = m_unit;
					m_player->ORDERS.push_back(order);

					if (++m_side_steps >= DRAW_SIDE_DIST)
						m_state = DrawingState::Return;
				}
				break;
			}
			case DrawingState::Return:
			{
				Position src { unit(m_unit).position() };
				if (ENABLE_FILLER)
				{
					if (TryFiller(src))
						return;
				}

				vector<Position> source { src };
				
				bool found { false };
				Position target{};
				Direction target_dir{ Direction::null };

				LayeredBFS bfs {
					MAX_DRAW_BFS,
					source,
					// Visitor
					[me = m_player->me(), &found, &target, &target_dir](Square const& sq, Direction first_dir, int index, int distance)
					{
						if (found)
							return;

						if (sq.painted() && sq.painter() == me)
						{
							target = sq.pos();
							target_dir = first_dir;
							found = true;
						}
					},
					// Depth completion
					[&found](int index) -> bool
					{
						return found;
					},
					// Queuer
					[this](Square const& sq) -> std::vector<Direction>
					{
						std::vector<Direction> result;

						for (auto&& dir : m_player->DIRS_STRAIGHT)
						{
							Position aux = sq.pos() + dir;
							if (posOk(aux) && (!square(aux).drawn() || square(aux).drawer() != m_player->me()))
								result.push_back(dir);
						}

						return result;
					}
				};
				bfs.run();

				Position next = src + target_dir;
				Square const& sq_next = square(next);
				if (sq_next.painted() && sq_next.painter() == m_player->me())
				{
					m_state = DrawingState::None;
				}

				// Go to the position we have found
				MyOrder order {};
				order.target_pos = src+target_dir;
				order.priority = PRIO_DRAW;
				order.order.dir = target_dir;
				order.order.type = OrderType::movement;
				order.order.unitId = m_unit;
				m_player->ORDERS.push_back(order);

				break;
			}
			
			default:
				m_state = DrawingState::Return;
				m_away_steps = 0;
				if (ENABLE_FILLER)
				{
					if (TryFiller(act))
						return;
				}
				break;
			}
		}
		else // not drawing
		{
			m_state = DrawingState::None;

			Square const& sq_act = square(unit(m_unit).position());
			if (sq_act.painted() && sq_act.painter() == m_player->me())
			{
				// Inside
				Position src { sq_act.pos() };

				if (ENABLE_FILLER)
				{
					if (TryFiller(act))
						return;
				}

				vector<Position> source { src };

				bool found { false };
				Position target{};
				Direction target_dir{ Direction::null };

				LayeredBFS bfs {
					MAX_DRAW_BFS,
					source,
					// Visitor
					[me = m_player->me(), &found, &target, &target_dir](Square const& sq, Direction first_dir, int index, int distance)
					{
						if (found)
							return;

						if (!sq.painted() || sq.painter() != me)
						{
							target = sq.pos();
							target_dir = first_dir;
							found = true;
						}
					},
					// Depth completion
					[&found](int index) -> bool
					{
						return found;
					},
					// Queuer
					[this](Square const& sq) -> std::vector<Direction>
					{
						std::vector<Direction> result;

						for (auto&& dir : m_player->DIRS_ALL)
						{
							Position aux = sq.pos() + dir;
							if (!posOk(aux))
								continue;	

							Square const& sq_aux = square(aux);
							if (sq_aux.drawn() && sq_aux.drawer() == m_player->me())
								continue;

							if (sq_aux.hasUnit() && sq_aux.unit().player() == m_player->me())
								continue;

							if (utils::isDiagonal(dir) && (!sq_aux.painted() || sq_aux.painter() != m_player->me()))
								continue;

							result.push_back(dir);
						}

						return result;
					}
				};
				bfs.run();

				// Go to the position we have found
				Position next = src + target_dir;
				if (!square(next).painted() || square(next).painter() != m_player->me())
				{
					// Start drawing
					m_state = DrawingState::Away;
					m_away_dir = target_dir;
					m_away_steps = 1;
				}

				MyOrder order {};
				order.target_pos = src+target_dir;
				order.priority = PRIO_DRAW;
				order.order.dir = target_dir;
				order.order.type = OrderType::movement;
				order.order.unitId = m_unit;
				m_player->ORDERS.push_back(order);
			}
			else
			{
				// Outside
				Position src { unit(m_unit).position() };
				vector<Position> source { src };
				
				bool found { false };
				Position target{};
				Direction target_dir { Direction::null };

				LayeredBFS bfs {
					MAX_DRAW_BFS,
					source,
					// Visitor
					[me = m_player->me(), &found, &target, &target_dir](Square const& sq, Direction first_dir, int index, int distance)
					{
						if (found)
							return;

						if (sq.painted() && sq.painter() == me)
						{
							target = sq.pos();
							target_dir = first_dir;
							found = true;
						}
					},
					// Depth completion
					[&found](int index) -> bool
					{
						return found;
					},
					// Queuer
					[this](Square const& sq) -> std::vector<Direction>
					{
						std::vector<Direction> result;

						for (auto&& dir : m_player->DIRS_STRAIGHT)
						{
							Position aux = sq.pos() + dir;
							if (!posOk(aux))
								continue;	

							Square const& sq_aux = square(aux);
							if (sq_aux.drawn() && sq_aux.drawer() == m_player->me())
								continue;

							if (sq_aux.hasUnit() && sq_aux.unit().player() == m_player->me())
								continue;

							result.push_back(dir);
						}

						return result;
					}
				};
				bfs.run();

				// Go to the position we have found
				MyOrder order {};
				order.target_pos = src+target_dir;
				order.priority = PRIO_DRAW;
				order.order.dir = target_dir;
				order.order.type = OrderType::movement;
				order.order.unitId = m_unit;
				m_player->ORDERS.push_back(order);
			}
		}
	}

	bool Busy() const
	{
		return m_state != DrawingState::None;
	}
private:
	bool IsDrawing() const
	{
		Square const& sq = square(unit(m_unit).position());
		return sq.drawn() && sq.unitDrawer() == m_unit;
	}

	bool TryFiller(Position act)
	{
		/* Try to find a Square we own that:
		- Is close to us if we allow any move
		- Is far from us if we only allow moving on our squares

		This should identify bottleneck-like structures and do intelligent drawings
		*/
		Distancer distancer 
		{
			100,
			act,
			[this, me = m_player->me()](Square const& sq) -> std::vector<Direction>
			{
				std::vector<Direction> result;

				for (auto&& dir : m_player->DIRS_STRAIGHT)
				{
					Position next { sq.pos() + dir };
					if (posOk(next))
					{
						Square const& sq_next = square(next);
						if ((sq_next.painted() && sq_next.painter() == me) || (sq_next.drawn() && sq_next.unitDrawer() == m_unit && !sq_next.hasUnit()))
							result.push_back(dir);
					}
				}
				return result;
			}
		};

		double best_ratio { 1.0 };
		Position best_candidate { -1, -1 };
		Direction move_dir { Direction::null };
		std::vector<Position> source { act };
		LayeredBFS bfs
		{
			MAX_FILLER_CHECK,
			source,
			// Visitor
			[&distancer, me = m_player->me(), &best_ratio, &move_dir, &best_candidate, this]
			(Square const& sq, Direction first_dir, int index, int distance)
			{
				int connection_distance = distancer.Distance(sq.pos());
				double ratio = static_cast<double>(connection_distance)/static_cast<double>(distance);

				if (ratio > best_ratio)
				{
					best_candidate = sq.pos();
					best_ratio = ratio;
					move_dir = first_dir;
				}
			},
			// Depth completion
			[](int index) -> bool
			{
				return false;
			},
			// Queuer
			[this, me = m_player->me()](Square const& sq) -> std::vector<Direction>
			{
				std::vector<Direction> options;

				if (sq.painted() && sq.painter() == me)
					options = m_player->DIRS_ALL;
				else
					options = m_player->DIRS_STRAIGHT;

				std::vector<Direction> result;

				for (auto&& dir : options)
				{
					Position next { sq.pos() + dir};
					if (posOk(next))
					{
						Square const& sq_next = square(next);
						if (!m_player->FILLER_MARKS[next] && (!sq_next.drawn() || sq_next.drawer() != me))
							result.push_back(dir);
					}
				}
				return result;
			}
		};
		bfs.run();

		if (best_ratio >= MIN_FILLER_RATIO)
		{
			// Block zone from being accessed by others
			for (int row = best_candidate.x-2; row < best_candidate.x+2; ++row)
			{
				for (int col = best_candidate.y-2; col < best_candidate.y+2; ++col)
				{
					Position aux { row, col };
					if (posOk(aux))
						m_player->FILLER_MARKS[aux] = true;
				}
			}

			// Take the shortest path while also starting a drawing
			Position next { act + move_dir };
			Square const& sq_next = square(next);
			if (utils::isDiagonal(move_dir) && (!sq_next.painted() || sq_next.painter() != m_player->me()))
			{
				auto [dir_x, dir_y] = utils::decompose(move_dir);

				if (randomNumber(0,1) == 0)
					move_dir = dir_x;
				else
					move_dir = dir_y;
			}

			MyOrder order {};
			order.target_pos = act+move_dir;
			order.priority = PRIO_DRAW;
			order.order.dir = move_dir;
			order.order.type = OrderType::movement;
			order.order.unitId = m_unit;
			m_player->ORDERS.push_back(order);

			return true;
		}
		return false;
	}

	DrawingState m_state { DrawingState::None };
	int m_away_steps { 0 };
	int m_side_steps { 0 };
	Direction m_away_dir { Direction::null };
	Direction m_side_dir { Direction::null };
	PLAYER_NAME* m_player { nullptr };

	// Index of unit in MY_UNITS
	int m_unit { -1 };
};

/*
	Opening moves
*/
const int OPENING_ROUNDS = 27;

vector<Direction> UNIT0OPENING{
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
vector<Direction> UNIT1OPENING{
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
vector<Direction> UNIT2OPENING{
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
vector<Direction> UNIT3OPENING{
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
vector<Direction> UNIT4OPENING{
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
vector<Direction> UNIT5OPENING{
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
vector<Direction> UNIT6OPENING{
    Direction::UR,
    Direction::UR,
    Direction::up,
    Direction::up,
    Direction::up,
    Direction::up,
    Direction::up
};
vector<Direction> UNIT7OPENING{
    Direction::UR,
    Direction::UR,
    Direction::right,
    Direction::up,
    Direction::right
};

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
map<int, Drawing> DRAWINGS;
map<int, int> WAITS;
int DISTANCE_TO_BONUS{1000};
Matrix<bool> FILLER_MARKS{50, 50};

void play()
{
	if (round() == 0)
		Init(); 
	
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
	for (auto&& x : FILLER_MARKS)
		x = false;
}

/* Used by update */
void UpdateUnits()
{
	MY_UNITS = units(me());

	if (MY_UNITS.empty())
	{	
        BUSY.clear();
	}
    else if (MY_UNITS.back() >= BUSY.size())
	{
        BUSY.resize(MY_UNITS.back()+1);
	}

	for (auto&& x : BUSY)
		x = false;

	// Remove drawings of old units
	vector<int> to_erase{};
	for (auto it = DRAWINGS.begin(); it != DRAWINGS.end(); it++)
	{
		bool found = false;
		for (auto unit : MY_UNITS)
		{
			if (it->first == unit)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			to_erase.push_back(it->first);
		}
	}

	for (int x : to_erase)
		DRAWINGS.erase(x);

	// Add drawings for new units
	for (auto&& unit : MY_UNITS)
	{
		if (auto it = DRAWINGS.find(unit); it == DRAWINGS.end())
			DRAWINGS[unit] = Drawing(this, unit);
	}

	// Erase waits of dead units
	to_erase.clear();
	for (auto it = WAITS.begin(); it != WAITS.end(); ++it)
	{
		bool found = false;
		for (auto unit : MY_UNITS)
		{
			if (it->first == unit)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			to_erase.push_back(it->first);
		}
	}

	for (int x : to_erase)
		WAITS.erase(x);
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
		[&options, &sources, &index_map, me = me(), this](Square const& sq, Direction first_dir, int index, int distance)
		{
			auto& my_options = options[index];
			auto& source = sources[index];

			auto GetClose = [&source, &sq, me, this](MyOrder& option)
			{
				option.order.type = OrderType::movement;
				option.order.dir = source.to(sq.pos());
				option.priority = PRIO_GET_CLOSE;
				option.target_pos = sq.pos();

				Square const& current = square(source);
				if (utils::isDiagonal(option.order.dir) && (!current.painted() || current.painter() != me))
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
				option.target_pos = Position(-1, -1);

				if (distance == 1)
				{
					option.order.type = ENABLE_ATTACKS ? OrderType::attack : OrderType::movement;
					option.order.dir = source.to(sq.pos());
					option.priority = PRIO_ATTACK_1;
					option.target_pos = sq.pos();
					// Don't need to set unitId here
				}
				else if (distance >= 2)
				{
					Position vec { sq.pos() - source };
					Square const& sq_source = square(source);

					if (ENABLE_DOMAIN_EXPANSION && unit(index_map[index]).upgraded())
					{
						Position AtoB = sq.pos() - sq_source.pos();
						AtoB.x = abs(AtoB.x);
						AtoB.y = abs(AtoB.y);
						int enemy_distance = DistanceFromAToBAs(sq, sq_source, sq.unit().player());

						// We can trap this unit inside our ability
						if (enemy_distance > 1 && AtoB.x <= 2 && AtoB.y <= 2)
						{
							option.priority = PRIO_USE_ABILITY;
							option.target_pos = source;
							option.order.dir = Direction::null;
							option.order.type = OrderType::ability;
							// Don't need to set unitId here
							goto push;
						}
					}

					if (ENABLE_DISTANCE2_INTELLIGENCE && distance == 2)
					{
						int enemy_distance = DistanceFromAToBAs(sq, sq_source, sq.unit().player());

						if (distance <= enemy_distance) // Equal footing or advantage
						{
							// Attack for K rounds, diagonally if possible
							int& waited = WAITS[index_map[index]];

							if (waited < MAX_WAIT_ROUNDS)
							{
								++waited;

								Direction attack_dir = source.to(sq.pos());
								option.order.type = OrderType::attack;
								option.order.dir = attack_dir;
								option.priority = PRIO_ATTACK_WAIT;
								option.target_pos = source + attack_dir;
							}
							else // We have already waited (or we don't do that)
							{
								Direction move_dir = source.to(sq.pos());
								if (ENABLE_SAFE_APPROACH && utils::isDiagonal(move_dir) && enemy_distance == 3)
								{
									// Moving in diagonal would expose us to attacks
									auto [dir_x, dir_y] = utils::decompose(move_dir);
									move_dir = randomNumber(0,1) == 1 ? dir_x : dir_y;
								}

								option.order.type = OrderType::movement;
								option.order.dir = move_dir;
								option.priority = PRIO_GET_CLOSE;
								option.target_pos = source + move_dir;
							}
						}
						else // They have an edge on us
						{
							// Move
							option.priority = PRIO_AVOID_UNFAIR;
							option.order.type = OrderType::movement;

							if (ENABLE_BAD_DIAG_HANDLE)
							{
								// Run away
								auto [dir_x, dir_y] = utils::decompose(sq.pos().to(source));
								option.order.dir = randomNumber(0,1) == 1 ? dir_x : dir_y;
							}
							else
							{
								// Dumber -> Get close
								auto [dir_x, dir_y] = utils::decompose(source.to(sq.pos()));
								option.order.dir = randomNumber(0,1) == 1 ? dir_x : dir_y;
							}

							option.target_pos = source + option.order.dir;
						}
					}
					else if (!ENABLE_DISTANCE2_INTELLIGENCE)
					{
						GetClose(option);
					}
				}

				push:
				if (posOk(option.target_pos) && TARGETS.can(option.target_pos))
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

				if (posOk(option.target_pos) && TARGETS.can(option.target_pos))
				{
					my_options.push_back(option);
				}
			}
		
			if (ENABLE_STEP_ON_DRAWING && sq.drawn() && sq.drawer() != me)
			{
				MyOrder option;
				GetClose(option);
				option.priority = PRIO_STEP_ON_DRAWING;

				if (posOk(option.target_pos) && TARGETS.can(option.target_pos))
				{
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
						if (posOk(option.target_pos) && TARGETS.can(option.target_pos))
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
	if (!ENABLE_USEFUL_ABILITIES)
		return;

	int upgraded_id = -1;

	for (auto&& u : MY_UNITS)
	{
		if (unit(u).upgraded())
		{
			upgraded_id = u;
			break;
		}
	}

	if (upgraded_id == -1)
		return;

	if (BUSY[upgraded_id])
		return;

	Unit un = unit(upgraded_id);
	Position src = un.position();
	std::vector<Position> source { un.position() };
	bool found { false };
	bool easy { false };

	Direction move_dir { Direction::null };

	LayeredBFS bfs{
		100,
		source,
		// Visitor
		[&found, &easy, me = me(), &move_dir, src, upgraded_id, this]
		(Square const& sq, Direction first_dir, int index, int distance)
		{
			if (found)
				return;

			if (ENABLE_DOMAIN_EXPANSION && sq.hasUnit() && sq.unit().player() != me)
			{
				MyOrder order;
				order.target_pos = src + first_dir;
				order.priority = PRIO_GET_CLOSE;
				order.order.dir = first_dir;
				order.order.type = OrderType::movement;
				order.order.unitId = upgraded_id;

				if (TARGETS.can(order.target_pos))
				{
					TARGETS.add(order.target_pos);
					BUSY[upgraded_id] = true;
					ORDERS.push_back(order);
					found = true;
					easy = true;
					return;
				}
			}

			if (sq.drawn() && sq.drawer() != me)
			{
				if (distance <= 2)
				{
					// Nuke
					MyOrder order;
					order.target_pos = src;
					order.priority = PRIO_USE_ABILITY;
					order.order.dir = Direction::null;
					order.order.type = OrderType::ability;
					order.order.unitId = upgraded_id;

					found = true;
					easy = true;
					return;
				}
				else
				{
					// Get close
					MyOrder order;
					order.target_pos = src + first_dir;
					order.priority = PRIO_GET_CLOSE;
					order.order.dir = first_dir;
					order.order.type = OrderType::movement;
					order.order.unitId = upgraded_id;

					if (TARGETS.can(order.target_pos))
					{
						TARGETS.add(order.target_pos);
						BUSY[upgraded_id] = true;
						ORDERS.push_back(order);
						found = true;
						easy = true;
						return;
					}
				}
			}

			if (!sq.painted() || sq.painter() != me)
			{
				if (!ENABLE_REALLY_USEFUL_ABILITIES)
				{
					if (distance <= 2)
					{
						MyOrder order;
						order.target_pos = src;
						order.priority = PRIO_USE_ABILITY;
						order.order.dir = Direction::null;
						order.order.type = OrderType::ability;
						order.order.unitId = upgraded_id;
					}
					else
					{
						// Get close
						MyOrder order;
						order.target_pos = src + first_dir;
						order.priority = PRIO_GET_CLOSE;
						order.order.dir = first_dir;
						order.order.type = OrderType::movement;
						order.order.unitId = upgraded_id;

						if (TARGETS.can(order.target_pos))
						{
							TARGETS.add(order.target_pos);
							BUSY[upgraded_id] = true;
							ORDERS.push_back(order);
							found = true;
							easy = true;
							return;
						}
					}
					
					found = true;
					move_dir = first_dir;
					return;
				}

				found = true;
				move_dir = first_dir;
				return;
			}
		},
		// Depth completion
		[&found](int index) -> bool
		{
			return found;
		},
		// Queuer
		[this, me = me(), src](Square const& sq) -> std::vector<Direction>
		{
			std::vector<Direction> options;

			if (sq.painted() && sq.painter() == me)
			{
				options = DIRS_DIAGONAL;
			}

			for (auto&& dir : DIRS_STRAIGHT)
				options.push_back(dir);

			std::vector<Direction> result;

			for (auto&& dir : options)
			{
				Position aux = src + dir;
				if (posOk(aux))
				{
					Square const& sq_aux = square(aux);
					if (!sq_aux.drawn() || sq_aux.drawer() != me)
					{
						result.push_back(dir);
					}
				}
			}

			return result;
		}
	};
	bfs.run();

	// The closest thing is a square we don't own
	if (ENABLE_REALLY_USEFUL_ABILITIES && !easy)
	{
		Position next = src + move_dir;
		Square const& sq_next = square(next);
		if (sq_next.painted() && sq_next.painter() == me())
		{
			// Not there yet -> get close
			MyOrder order;
			order.target_pos = src + move_dir;
			order.priority = PRIO_GET_CLOSE;
			order.order.dir = move_dir;
			order.order.type = OrderType::movement;
			order.order.unitId = upgraded_id;

			ORDERS.push_back(order);
			TARGETS.add(order.target_pos);
			BUSY[upgraded_id] = true;
		}
		else
		{
			// Compute best spot to use the ability on
			std::vector<Position> source { src };

			int best_paint = 0;
			Position best_paint_pos{ -1, -1 };
			Direction move_dir = Direction::null;

			LayeredBFS bfs2
			{
				DISTANCE_TO_BONUS-2,
				source,
				// Visitor
				[&best_paint, &best_paint_pos, me = me(), this, &move_dir]
				(Square const& sq, Direction first_dir, int index, int distance)
				{
					Position pos = sq.pos();
					int begin_row = pos.x - 2;
					int end_row = pos.x + 2;
					int begin_col = pos.y - 2;
					int end_col = pos.y + 2;

					int paintable = 0;

					for (int i = begin_row; i < end_row; ++i)
					{
						for (int j = begin_col; j < end_col; ++j)
						{
							Position aux { i, j };
							if (!posOk(aux))
								continue;
							
							Square sq_aux = square(aux);
							if (!sq_aux.painted() || sq_aux.painter() != me)
								++paintable;
						}
					}

					if (paintable > best_paint)
					{
						best_paint = paintable;
						best_paint_pos = pos;
						move_dir = first_dir;
					}
				},
				// Depth completion
				[](int index) -> bool
				{
					return false;
				},
				// Queuer
				[this](Square const& sq) -> std::vector<Direction>
				{
					return DIRS_STRAIGHT;
				}
			};
			bfs2.run();

			MyOrder order;
			order.order.unitId = upgraded_id;
			order.order.dir = move_dir;

			if (move_dir == Direction::null)
			{
				order.order.type = OrderType::ability;
				order.target_pos = src;
				order.priority = PRIO_USE_ABILITY;
			}
			else
			{
				order.order.type = OrderType::movement;
				order.target_pos = src+move_dir;
				order.priority = PRIO_GET_CLOSE;
			}

			ORDERS.push_back(order);
			BUSY[upgraded_id] = true;
			TARGETS.add(order.target_pos);
		}
	}
}

void AssignUnitsToConsumables()
{	
	if (MAX_CONSUMABLE_BFS <= 0)
		return;

	/*
	LayeredBFS from consumable to free players

	When a free player has been allocated, pathfind to the consumable and assign thing
	*/

	std::map<int, Position> taken{};

	std::set<int> bonuses{};

	std::vector<Position> consumable_sources{};
	for (int i{0}; i < rows(); ++i)
	{
		for (int j{0}; j < cols(); ++j)
		{
			Position pos{i,j};
			Square const& sq = square(pos);

			if (sq.hasBonus())
			{
				bonuses.insert(consumable_sources.size());
				consumable_sources.push_back(pos);
			}
		
			if (sq.hasBubble())
				consumable_sources.push_back(pos);
		}
	}
	std::vector<bool> consumable_finished(consumable_sources.size(), false);
	std::vector<int> consumable_found(consumable_sources.size(), 100);
	int min_unit_to_bonus_distance = 1000;

	LayeredBFS bfs{
		MAX_CONSUMABLE_BFS,
		consumable_sources,
		// Visitor
		[&consumable_finished, &consumable_found, &consumable_sources, me = me(), this, &taken, &bonuses, &min_unit_to_bonus_distance]
		(Square const& sq, Direction first_dir, int index, int distance)
		{
			if (consumable_finished[index])
				return;

			if (distance - consumable_found[index] > MAX_CONSUMABLE_DIST_DIFF)
			{
				consumable_finished[index] = true;
				return;
			}

			if (sq.hasUnit())
			{
				Unit const& un = sq.unit();
				if (un.player() == me)
				{
					if (BUSY[un.id()])
						return;

					if (taken.find(un.id()) != taken.end())
						return;

					consumable_finished[index] = true;
					taken.insert(std::make_pair(un.id(), consumable_sources[index]));

					if (bonuses.find(index) != bonuses.end())
					{
						if (distance < min_unit_to_bonus_distance)
							min_unit_to_bonus_distance = distance;
					}
				}
				else
				{
					// Enemy unit
					consumable_found[index] = distance;				
				}
			}
		},
		// On depth completion
		[&consumable_finished](int index) -> bool
		{
			return consumable_finished[index];
		},
		// Queuer
		[this](Square const& sq) -> std::vector<Direction>
		{
			std::vector<Direction> result;

			for (auto&& dir : DIRS_DIAGONAL)
			{
				Position aux = sq.pos() + dir;
				if (posOk(aux))
				{
					Square const& sq_aux = square(aux);
					if (sq_aux.painted())
						result.push_back(dir);
				}
			}

			for (auto&& dir : DIRS_STRAIGHT)
				result.push_back(dir);

			return result;
		}
	};
	bfs.run();

	DISTANCE_TO_BONUS = min_unit_to_bonus_distance;

	for (auto it = taken.begin(); it != taken.end(); ++it)
	{
		int id = it->first;
		Position destination = it->second;
		Position current = unit(id).position();

		Direction dir = PathfindFromAToBAs(current, destination, me());

		// If we exit a painted area, avoid diagonals
		if (utils::isDiagonal(dir))
		{
			Position next = current + dir;
			if (posOk(next))
			{
				Square const& sq_next = square(next);
				
				if (!sq_next.painted() || sq_next.painter() != me())
				{
					auto [dir_x, dir_y] = utils::decompose(dir);
					dir = randomNumber(0,1) == 1 ? dir_x : dir_y;
				}
			}
		}

		MyOrder order;
		order.target_pos = current + dir;
		order.priority = PRIO_SEARCH;
		order.order.dir = dir;
		order.order.type = OrderType::movement;
		order.order.unitId = id;

		TARGETS.add(order.target_pos);
		ORDERS.push_back(order);
		BUSY[id] = true;
	}
}

void ManageDrawings()
{
	for (auto&& unit : MY_UNITS)
	{
		if (BUSY[unit])
			continue;

		DRAWINGS[unit].Process();
	}
}

void SendOrders()
{
	if (!ENABLE_PRIORITIES)
	{
		// Assign random values to each order before sorting
		for (auto&& ord : ORDERS)
			ord.priority = randomNumber(0, 200);
	}

	// Invert priority of things that should happen last
	for (auto&& ord : ORDERS)
	{
		if (ord.priority == PRIO_ATTACK_WAIT)
		{
			ord.priority = -PRIO_ATTACK_WAIT;
		}
	}

	std::sort(ORDERS.begin(), ORDERS.end());

	for (auto&& ord : ORDERS)
	{
		switch (ord.order.type)
		{
		case OrderType::movement:
			WAITS[ord.order.unitId] = 0;
			move(ord.order.unitId, ord.order.dir);
			break;
		case OrderType::attack:
			attack(ord.order.unitId, ord.order.dir);
			break;
		case OrderType::ability:
			WAITS[ord.order.unitId] = 0;
			ability(ord.order.unitId);
			break;
		}
	}
}

/*
	UTILITY
*/

/*  Returns the distance from A to B assuming a unit of the given player.
	This basically means "Manhattan distance but step diagonally whenever you can" */
int DistanceFromAToBAs(Square const& A, Square const& B, int player)
{
	std::vector<Position> source { A.pos() };
	int result { 0 };
	bool found { false };

	LayeredBFS bfs {
		100,
		source,
		// Visitor
		[&found, &result, &B](Square const& sq, Direction first_dir, int index, int distance)
		{
			if (found)
				return;

			if (sq.pos() == B.pos())
			{
				found = true;
				result = distance;
			}
		},
		// Depth completion
		[&found](int index) -> bool
		{
			return found;
		},
		// Queuer
		[player, this, &found](Square const& sq) -> std::vector<Direction>
		{
			std::vector<Direction> result{};

			if (sq.painted() && sq.painter() == player)
				result = DIRS_DIAGONAL;

			for (auto&& dir : DIRS_STRAIGHT)
				result.push_back(dir);

			return result;
		}
	};
	bfs.run();

	return result;
}

/*  Returns the direction a unit of the given player has to take in a movement to go from A to B, assuming
	also that the unit is located on A. */
Direction PathfindFromAToBAs(Position A, Position B, int player)
{
	std::vector<Position> source { A };
	Direction result { Direction::null };
	bool found { false };

	LayeredBFS bfs {
		100,
		source,
		// Visitor
		[&found, &result, &B](Square const& sq, Direction first_dir, int index, int distance)
		{
			if (found)
				return;

			if (sq.pos() == B)
			{
				found = true;
				result = first_dir;
			}
		},
		// Depth completion
		[&found](int index) -> bool
		{
			return found;
		},
		// Queuer
		[player, this, &found](Square const& sq) -> std::vector<Direction>
		{
			std::vector<Direction> candidates{};

			if (sq.painted() && sq.painter() == player)
				candidates = DIRS_DIAGONAL;

			for (auto&& dir : DIRS_STRAIGHT)
				candidates.push_back(dir);

			std::vector<Direction> result{};
			for (auto&& dir : candidates)
			{
				Position next = sq.pos() + dir;
				if (!posOk(next))
					continue;

				Square const& sq_next = square(next);
				// Drawings are walls
				if (!sq_next.drawn() || sq_next.drawer() != player)
					result.push_back(dir);
			}

			return result;
		}
	};
	bfs.run();

	return result;
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