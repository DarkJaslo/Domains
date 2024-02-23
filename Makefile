# Detect all native players (files matching "AI*.cc")

PLAYERS_DIR := ./src/bots
SRC_DIR := ./src/game
OBJ_DIR := ./obj
EXE := Game.exe

PLAYERS_SRC = $(wildcard $(PLAYERS_DIR)/AI*.cc)
PLAYERS_OBJ = $(patsubst %.cc, %.o, $(PLAYERS_SRC)) #$(EXTRA_OBJ) $(DUMMY_OBJ)
SRC = $(wildcard $(SRC_DIR)/*.cc)
OBJ = $(SRC:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

DEBUG = -g -O0 -fno-inline -D_GLIBCXX_DEBUG
RUN = -O3

CXX = g++
CXXFLAGS = -Wall -Wno-sign-compare -std=c++17 $(RUN)
LDFLAGS =

all: $(EXE) 

$(EXE): $(OBJ) $(PLAYERS_OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(PLAYERS_SRC) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR) :
	mkdir -p $@

clean: 
	rm src/bots/*.o obj/*.o *.exe
	@rm -rv $(OBJ_DIR)

#-include $(OBJ:.o=.d)