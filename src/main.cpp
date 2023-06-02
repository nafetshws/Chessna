#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"

int main(){
    std::string fen = "8/4k3/8/2B3Q1/8/8/8/8 w - - 0 1";
    Board board(DEFAULT_FEN);
    
    MoveGeneration moveGeneration(board);
    std::vector<Move> moves = moveGeneration.generateMoves();

    std::cout << "number of moves: " << moves.size() << std::endl;

    return 0;
}