#include "Player.hh"
#include <iostream>

//Player
int Player::me()const{return id_;}
Player::~Player(){
  std::cerr << "deleting player" << std::endl;
}