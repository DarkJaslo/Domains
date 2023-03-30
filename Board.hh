#include "GameInfo.hh"
#include "Player.hh"

class Board{
public:
  //Default constructor
  Board();
  //Reads settings and initializes variables
  void iniBoard(int s);
  //Executes the orders given in this round
  void executeRound(const vector<Player*>& pl);
  //Prints the results of this round
  void printRound();
  //Prints all game settings
  void printSettings();
private:
  struct Order{
    int unitId;
    Direction dir;
    OrderType type;
    Order(int id,Direction d, OrderType t){unitId = id; dir = d; t = type;}
  };
  //True if uid is a valid unit, false if it is not
  bool unitOk(int uid)const;

  //Erases all drawings made by uid in p and all connected squares
  void erasePath(int uid, Position p);

  //Updates everything to match u's death
  void killUnit(Unit& u);

  void enclose(int plId, int uid, Position p, int& xmin, int& xmax, int& ymin, int& ymax);

  void flood(int plId, int col, Position p, bool& ok, vector<vector<Square>>& grid);

  //Seriously how the fuck do you do this
  void paint(int plId, int uid, Position p);

  //Tries to draw. If in your own territory, it does not. If it steps on a drawing, it tries to paint and erases it. If none of this happens, draws the square at pnew
  void draw(int plId, int uid, Position pnew, Position pant);

  //Executes the fight between u1 and u2 and its consequences... Returns the winner's uid
  int fight(Unit& u1, Unit& u2);

  //Executes ord (if it is valid) and returns true if it succeeds
  bool executeOrder(int plId, const Order& ord);

  int seed;
  GameInfo info;
  vector<bool> killedUnits; 
  //vector<Player*> players;
};