#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    //std::string fen = "4k3/3n1p2/8/1Q2r2B/8/8/8/2K1R3 b - - 0 1";
    std::string fen = "6k1/8/3r4/6b1/1q1BN3/2P5/3KR2r/r7 w - - 0 1";
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    std::vector<Move> moves = moveGeneration.generateMoves(WHITE);

    printMoves(moves);


    return 0;
}