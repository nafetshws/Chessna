#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    //std::string fen = DEFAULT_FEN; 
    std::string fen = "rnbqkb1r/pppppp1p/7n/6pP/8/8/PPPPPPP1/RNBQKBNR w KQkq g6 0 1"; 
    Board board(fen);
    
    MoveGeneration moveGeneration(board);

    //std::vector<Move> moves = moveGeneration.generateMoves(WHITE);
    //printMoves(moves);

    u64 nodes = moveGeneration.perft(2, WHITE);

    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}