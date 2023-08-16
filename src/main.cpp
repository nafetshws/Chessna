#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    std::string fen = DEFAULT_FEN; 
    //std::string fen = "rnb1kbnr/pp1ppppp/2p5/q7/8/3P4/PPPKPPPP/RNBQ1BNR w HAkq - 0 1"; 
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    u64 nodes = moveGeneration.perft(4, WHITE);

    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}