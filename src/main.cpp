#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    //std::string fen = DEFAULT_FEN; 
    std::string fen = "1nbqkbnr/rppppppp/p3Q3/8/8/2P5/PP1PPPPP/RNB1KBNR b KQk - 0 1"; 
    Board board(fen);
    
    MoveGeneration moveGeneration(board);

    //std::vector<Move> moves = moveGeneration.generateMoves(WHITE);
    //printMoves(moves);

    u64 nodes = moveGeneration.perft(1, BLACK);

    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}