main:
	g++ -o build/main.obj src/main.cpp src/board.cpp src/moveGeneration.cpp 
tests:
	g++ -o build/test.obj test/test.cpp src/board.cpp src/moveGeneration.cpp
clean:
	rm -rf build/main.obj build/test.obj
