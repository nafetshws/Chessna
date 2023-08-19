#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    //std::string fen = DEFAULT_FEN; 
    std::string fen = "rnbqkb1r/pppppppp/7n/8/7P/P7/1PPPPPP1/RNBQKBNR b KQkq - 0 1"; 
    Board board(fen);
    
    MoveGeneration moveGeneration(board);

    u64 nodes = moveGeneration.perft(2, BLACK);

    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}