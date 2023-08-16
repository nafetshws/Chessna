#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    //std::string fen = DEFAULT_FEN; 
    std::string fen = "rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNB1KBNR b KQkq - 0 1"; 
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    u64 nodes = moveGeneration.perft(1, BLACK);

    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}