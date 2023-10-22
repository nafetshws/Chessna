CC = g++

TEST_FILES = test/test.cpp src/board.cpp src/moveGeneration.cpp src/functions.cpp src/search.cpp src/evaluation.cpp src/gameInterface.cpp src/moveOrder.cpp src/transpositionTable.cpp src/zobrist.cpp src/uci.cpp
FILES = src/main.cpp src/board.cpp src/moveGeneration.cpp src/functions.cpp src/search.cpp src/evaluation.cpp src/gameInterface.cpp src/moveOrder.cpp src/transpositionTable.cpp src/zobrist.cpp src/uci.cpp

TEST_OUT = build/test.obj
OUT = build/main.obj
DEBUG_OUT = build/debug.obj

CC_FLAGS = -std=c++11 -O3

BUILD_DIR = ./build

main:
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	g++ -o $(OUT) $(FILES) $(CC_FLAGS) 
tests:
	g++ -o $(TEST_OUT) $(TEST_FILES) $(CC_FLAGS)
debug:
	g++ -g -o $(DEBUG_OUT) $(FILES) $(CC_FLAGS) 


.PHONY: clean
clean:
	rm -rf $(OUT) $(TEST_OUT)
