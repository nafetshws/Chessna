CC = g++

TEST_FILES = test/test.cpp src/board.cpp src/moveGeneration.cpp src/functions.cpp src/search.cpp src/evaluation.cpp src/gameInterface.cpp src/moveOrder.cpp src/transpositionTable.cpp src/zobrist.cpp src/uci.cpp src/repetitionTable.cpp
FILES = src/main.cpp src/board.cpp src/moveGeneration.cpp src/functions.cpp src/search.cpp src/evaluation.cpp src/gameInterface.cpp src/moveOrder.cpp src/transpositionTable.cpp src/zobrist.cpp src/uci.cpp src/repetitionTable.cpp

TEST_OUT = build/test
OUT = build/chessna
DEBUG_OUT = build/debug

CC_FLAGS = -std=c++11 -O3 -pthread

BUILD_DIR = ./build

main:
	g++ -o $(OUT) $(FILES) $(CC_FLAGS) 
tests:
	g++ -o $(TEST_OUT) $(TEST_FILES) $(CC_FLAGS)
debug:
	g++ -g -o $(DEBUG_OUT) $(FILES) $(CC_FLAGS) -Wall 


.PHONY: clean
clean:
	rm -rf $(OUT) $(TEST_OUT) $(DEBUG_OUT)
