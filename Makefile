main:
	g++ -o build/main.obj src/main.cpp src/board.cpp src/MoveGeneration.cpp 
clean:
	rm -rf build/main.obj
