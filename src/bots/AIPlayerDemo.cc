//Must include this
#include "../game/Player.hh"

#include <vector>

//Your player's name goes here. Do not use "Player" and "Dummy". In this case, AIPlayerDemo.cc -> PlayerDemo
#define PLAYER_NAME PlayerDemo

//Has to inherit from Player
struct PLAYER_NAME : public Player
{
    //Keep this here and never remove it
    static Player* factory(){ return new PLAYER_NAME; }

    // Global variables can be declared here

    // Will be called every round. Give an order to each unit here.
    void play()
    {
        //Simple example. Orders all units to move in a random direction if the current round is an even number

        auxiliarFunction();

        std::vector<int> myUnits = units(me());

        if(round()%2 != 0) return;
        
        for(int u : myUnits)
        {
            int random = randomNumber(1,4);
            if(random == 1) 
                move(u,Direction::up);
            else if(random == 2)
                move(u,Direction::down);
            else if(random == 3)
                move(u,Direction::left);
            else if(random == 4)
                move(u,Direction::right);
        }
    }

    void auxiliarFunction()
    {
        //Do stuff
    }
};

//Keep this here and never remove it. It is used to add this player to the executable when the game is compiled
REGISTER_PLAYER(PLAYER_NAME);