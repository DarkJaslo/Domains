# Detect all native players (files matching "AI*.cc")

PLAYERS_SRC = $(wildcard AI*.cc)
PLAYERS_OBJ = $(patsubst %.cc, %.o, $(PLAYERS_SRC)) #$(EXTRA_OBJ) $(DUMMY_OBJ)

# Rules
COMP = g++
CC = Register.cc Player.cc Board.cc Utility.cc PlayerOrders.cc GameInfo.cc Game.cc Main.cc
OBJ = Register.o Player.o Board.o Utility.o PlayerOrders.o GameInfo.o Game.o Main.o 
CFLAGS = -Wall -O3 -Wno-sign-compare

Game.exe:${OBJ} ${PLAYERS_OBJ}
	${COMP} ${CFLAGS} -o $@ ${OBJ} ${PLAYERS_OBJ}

clean: 
	rm *.o *.exe

.cc.o:
	${COMP} ${CFLAGS} -c $<