CC = g++

TEST_FILES = test/test.cpp src/board.cpp src/moveGeneration.cpp
FILES = src/main.cpp src/board.cpp src/moveGeneration.cpp 

TEST_OUT = build/test.obj
OUT = build/main.obj

main:
	g++ -o $(OUT) $(FILES) 
tests:
	g++ -o $(OUT) $(TEST_FILES) 

.PHONY: clean
clean:
	rm -rf $(OUT) $(TEST_OUT)
