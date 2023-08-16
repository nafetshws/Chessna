#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    //std::string fen = DEFAULT_FEN; 
    std::string fen = "rnbqkbnr/pppppp1p/8/6p1/7P/8/PPPPPPP1/RNBQKBNR w KQkq - 0 1"; 
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    u64 nodes = moveGeneration.perft(2, WHITE);

    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}