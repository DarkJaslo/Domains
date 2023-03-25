# The following two lines will detect all your players (files matching "AI*.cc")

PLAYERS_SRC = $(wildcard AI*.cc)
PLAYERS_OBJ = $(patsubst %.cc, %.o, $(PLAYERS_SRC)) #$(EXTRA_OBJ) $(DUMMY_OBJ)

#Rules

OBJ = Register.o Game.o Main.o Player.o

all: Game

Game:	$(OBJ) $(PLAYERS_OBJ)
	g++ $(OBJ) $(PLAYERS_OBJ) -o Game.exe

clean: 
	rm *.o *.exe