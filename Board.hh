#include "GameInfo.hh"
#include "Player.hh"

class Board{
public:
  Board();
  void iniBoard(int s);
  void executeRound(const vector<Player*>& pl);
  void printRound();
  void printSettings();
private:
  struct Order{
    int unitId;
    Direction dir;
    OrderType type;
    Order(int id,Direction d, OrderType t){unitId = id; dir = d; t = type;}
  };
  bool unitOk(int uid)const;
  void erasePath(int uid, const Position& p);
  void killUnit(Unit& u);
  void draw(int plId, int uid, const Position& p);
  int fight(Unit& u1, Unit& u2);
  bool executeOrder(int plId, const Order& ord);
  int seed;
  GameInfo info;
  vector<bool> killedUnits; 
  //vector<Player*> players;
};