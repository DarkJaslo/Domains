# Detect all native players (files matching "AI*.cc")

PLAYERS_SRC = $(wildcard AI*.cc)
PLAYERS_OBJ = $(patsubst %.cc, %.o, $(PLAYERS_SRC)) #$(EXTRA_OBJ) $(DUMMY_OBJ)

#Rules
CC = Register.cc Player.cc Board.cc Utility.cc PlayerOrders.cc GameInfo.cc Game.cc Main.cc
OBJ = Register.o Player.o Board.o Utility.o PlayerOrders.o GameInfo.o Game.o Main.o 

all: Gall
game: Register Player Board Utility PlayerOrders GameInfo Game Main Players exe

Register:	
	g++ -O3 -c Register.cc -o Register.o 

Player:	
	g++ -O3 -c Player.cc -o Player.o 

Board:	
	g++ -O3 -c Board.cc -o Board.o 

Utility:	
	g++ -O3 -c Utility.cc -o Utility.o 

PlayerOrders:	
	g++ -O3 -c PlayerOrders.cc -o PlayerOrders.o 

GameInfo:	
	g++ -O3 -c GameInfo.cc -o GameInfo.o 

Game:	
	g++ -O3 -c Game.cc -o Game.o 

Main:	
	g++ -O3 -c Main.cc -o Main.o

Players:
	g++ -O3 -c $(PLAYERS_SRC) -o $(PLAYERS_OBJ)

exe:
	g++ -O3 $(OBJ) $(PLAYERS_OBJ) -o Game.exe

Gall:	$(OBJ) $(PLAYERS_OBJ)
	g++ $(OBJ) $(PLAYERS_OBJ) -o Game.exe

opt:	$(OBJ) $(PLAYERS_OBJ)
	g++ -O3 -c $(CC) 
	g++ -O3 $(OBJ) $(PLAYERS_OBJ) -o Game.exe

clean: 
	rm *.o *.exe