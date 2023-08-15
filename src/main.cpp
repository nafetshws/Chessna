#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    std::string fen = "5k2/8/8/K2Pp2r/8/8/8/8 w - d3 0 1";
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    std::vector<Move> moves = moveGeneration.generateMoves(WHITE);

    printMoves(moves);


    return 0;
}