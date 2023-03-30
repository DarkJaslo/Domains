# Detect all native players (files matching "AI*.cc")

PLAYERS_SRC = $(wildcard AI*.cc)
PLAYERS_OBJ = $(patsubst %.cc, %.o, $(PLAYERS_SRC)) #$(EXTRA_OBJ) $(DUMMY_OBJ)

#Rules
CC = Register.cc Player.cc Board.cc Utility.cc PlayerOrders.cc GameInfo.cc Game.cc Main.cc
OBJ = Register.o Player.o Board.o Utility.o PlayerOrders.o GameInfo.o Game.o Main.o 

all: Game

Game:	$(OBJ) $(PLAYERS_OBJ)
	g++ $(OBJ) $(PLAYERS_OBJ) -o Game.exe

opt:	$(OBJ) $(PLAYERS_OBJ)
	g++ -O3 -c $(CC) 
	g++ -O3 $(OBJ) $(PLAYERS_OBJ) -o Game.exe

clean: 
	rm *.o *.exe