#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    //std::string fen = DEFAULT_FEN; 
    std::string fen = "r1bqkbnr/pppppppp/n7/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1"; 
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    u64 nodes = moveGeneration.perft(2, WHITE);

    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}