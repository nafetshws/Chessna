#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    std::string fen = DEFAULT_FEN; 
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    u64 nodes = moveGeneration.perft(5, WHITE);

    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}