#include "GameInfo.hh"
#include "Player.hh"

class Board{
public:
  ~Board();
  //Default constructor
  Board();
  //Debug and view constructor
  Board(bool d, bool v, int nplayers);
  //Reads settings and initializes variables
  void iniBoard(int s, int rounds);
  //Executes the orders given in this round
  void executeRound(const std::vector<Player*>& pl);
  //Prints the results of this round
  void printRound();
  //Prints all game settings
  void printSettings();

  void printPainters();

private:
  friend class Game;
  //True if uid is a valid unit, false if it is not
  bool unitOk(int uid)const;

  //Recursive func for erasePath
  void i_erasePath(int uid, Position p);

  //Erases all drawings made by uid in p and all connected squares
  void erasePath(int uid, Position p);

  //Updates everything to match u's death
  void killUnit(Unit& u);

  //enclose marks squares and this function removes those marks
  void deenclose(Position p);

  //Saves a rectangle (Position(xmin,ymin),Position(xmax,ymax)) to paint
  void enclose(int plId, int uid, Position p, int& xmin, int& xmax, int& ymin, int& ymax);

  void floodv2(int plId, int uid, int col, Position p, bool& correct, Matrix<Square>& grid);

  void perpendicularDirections(Direction dir, Direction& res1, Direction& res2);

  Position followDrawing(int uid, Position act, Position ant, Matrix<Square>& grid);

  //Seriously how the fuck do you do this

  void paintv2(int plId, int uid, Position in, Position out);

  //Tries to draw. If in your own territory, it does not. If it steps on a drawing, it tries to paint and erases it. If none of this happens, draws the square at pnew
  void draw(int plId, int uid, Position pnew, Position pant);

  enum FightMode{
    Fair,     //Both units can attack
    Attacks,  //Unit U1 can attack, Unit U2 can't
  };

  //Executes the fight between u1 and u2 and its consequences... Returns the winner's uid
  int fight(Unit& u1, Unit& u2, FightMode fm);

  //Executes ord (if it is valid) and returns true if it succeeds
  bool executeOrder(int plId, Order ord);

  //Performs all free attacks
  void performFreeAttacks();

  //Pops Bubble with bubble id = bid
  void popBubble(Position p, bool isForced);

  //Decreases all Bubbles' rtps and pops them if needed
  void popBubbles();

  //Invalidates the isAbility stat of a grid made by an ability
  void invalidateAbility(int plId, Position p);

  //Uses ability
  void useAbility(int plId, Position p);

  //Decides if queued abilities are used or not and uses them
  void resolveAbilities();

  //Gives points from painted squares
  void giveBoardPoints();

  //Moves all bubbles
  void moveBubbles();

  //Saves all squares owned by each player and decrements ability counters
  void getPlayerSquares();

  //Tries to respawn player units, bubbles and bonuses
  void respawn();

  //Adds or removes energy to all units
  void computeEnergies();

  //Spawns a player, giving them the unit identified with plId
  void spawnPlayer(int i, int j, int plId);

  //Spawns players
  void spawnPlayers();

  //Prints the start squares of all units
  void printStartSquares(const std::vector<std::pair<Position,Position>>& sqs);

  int seed;
  GameInfo info;
  std::vector<bool> killedUnits;
  std::vector<int> abilityUnits; //Stores which units have used the ability
    //Order management

  struct FreeAttack{
    int uid;
    Position uPos;
    Position tPos;
    FreeAttack(int id, Position up, Position tp){
      uid = id;
      uPos = up;
      tPos = tp;
    }
  };

  std::queue<FreeAttack> freeAttacks; 
  std::vector<Position> attackedPositions;
  std::vector<std::pair<Position,Position>> drawStarts;  //First position is origin, second is first drawed Square
  bool debug,view;
  bool paintDebug;
  bool debugDrawErase;
  bool debugOrders;
  bool debugBasic;
};