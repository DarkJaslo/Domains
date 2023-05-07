#include "GameInfo.hh"
#include "Player.hh"

class Board{
public:
  //Default constructor
  Board();
  //Debug and view constructor
  Board(bool d, bool v, int nplayers);
  //Reads settings and initializes variables
  void iniBoard(int s, int rounds);
  //Executes the orders given in this round
  void executeRound(const vector<Player*>& pl);
  //Prints the results of this round
  void printRound();
  //Prints all game settings
  void printSettings();
private:
  friend class Game;
  //True if uid is a valid unit, false if it is not
  bool unitOk(int uid)const;

  //Erases all drawings made by uid in p and all connected squares
  void erasePath(int uid, Position p);

  //Updates everything to match u's death
  void killUnit(Unit& u);

  //enclose marks squares and this function removes those marks
  void deenclose(Position p);

  //Saves a rectangle (Position(xmin,ymin),Position(xmax,ymax)) to paint
  void enclose(int plId, int uid, Position p, int& xmin, int& xmax, int& ymin, int& ymax);

  void flood(int plId, int col, Position p, bool& flooded, bool& ok, vector<vector<Square>>& grid);

  //Seriously how the fuck do you do this
  void paint(int plId, int uid, Position p);

  //Tries to draw. If in your own territory, it does not. If it steps on a drawing, it tries to paint and erases it. If none of this happens, draws the square at pnew
  void draw(int plId, int uid, Position pnew, Position pant);

  enum FightMode{
    Fair,     //Both units can attack
    Attacks, //Unit U1 can attack, Unit U2 can't
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

  int seed;
  GameInfo info;
  vector<bool> killedUnits;
  vector<int> abilityUnits; //Stores which units have used the ability
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
  queue<FreeAttack> freeAttacks; 
  vector<Position> attackedPositions;
  bool debug,view;
};