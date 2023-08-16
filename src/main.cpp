#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    //std::string fen = DEFAULT_FEN; 
    std::string fen = "rnbqkbnr/1ppppppp/8/p7/8/7N/PPPPPPPP/RNBQKB1R b KQkq - 0 1"; 
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    u64 nodes = moveGeneration.perft(1, WHITE);

    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}