CC = g++

TEST_FILES = test/test.cpp src/board.cpp src/moveGeneration.cpp src/functions.cpp
FILES = src/main.cpp src/board.cpp src/moveGeneration.cpp src/functions.cpp src/search.cpp 

TEST_OUT = build/test.obj
OUT = build/main.obj

CC_FLAGS = -std=c++11

BUILD_DIR = ./build

main:
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	g++ -o $(OUT) $(FILES) $(CC_FLAGS) 
tests:
	g++ -o $(TEST_OUT) $(TEST_FILES) $(CC_FLAGS)

.PHONY: clean
clean:
	rm -rf $(OUT) $(TEST_OUT)
