# For choosing players with a bash script
EXTRA_SRCS ?=
OUT ?= Game

PLAYERS_DIR := ./src/bots
SRC_DIR := ./src/game
OBJ_DIR := ./obj

PLAYERS_SRC = $(wildcard $(PLAYERS_DIR)/AI*.cc) $(EXTRA_SRCS)
PLAYERS_OBJ = $(patsubst %.cc, %.o, $(PLAYERS_SRC))
SRC = $(wildcard $(SRC_DIR)/*.cc)
OBJ = $(SRC:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

DEBUG = -g -O0 -fno-inline -D_GLIBCXX_DEBUG
RUN = -O3

CXX = g++
CXXFLAGS = -Wall -Wno-sign-compare -std=c++17 $(RUN)
LDFLAGS =

all: $(OUT).exe

$(OUT).exe: $(OBJ) $(PLAYERS_OBJ)
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