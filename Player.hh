#include "Register.hh"

class Player{

  public:
  int me() const;
  int squares() const;
  virtual void play(); 

  private:
  friend class Game;
  int id;
  int numSquares; 
};