#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    std::string fen = "8/8/8/8/k2Pp2Q/8/8/4K3 w - d3 0 1";
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    std::vector<Move> moves = moveGeneration.generateMoves(BLACK);

    printMoves(moves);


    return 0;
}