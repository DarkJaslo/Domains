#ifndef Player_hh
#define Player_hh

#include "Register.hh"
#include "PlayerOrders.hh"
#include "GameInfo.hh"

class Player : public PlayerOrders, public GameInfo{

  public:
  int me() const;
  virtual void play() = 0; 

  private:
  int id_;
  friend class Game;
  friend class Board;
};

#endif