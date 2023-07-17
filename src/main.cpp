#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    //std::string fen = "8/8/8/2k5/3Pp3/8/8/3K4 b - d3 0 1";
    std::string fen = "3k4/8/2pp1p2/1P1BN1N1/8/2P2P2/8/3K4 w - - 0 1";
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    std::vector<Move> moves = moveGeneration.generateMoves(BLACK);

    printMoves(moves);


    return 0;
}