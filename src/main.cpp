#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    std::string fen = DEFAULT_FEN; 
    //std::string fen = "rnbqk1nr/pppp1ppp/4p3/8/1b6/P2P4/1PP1PPPP/RNBQKBNR w KQkq - 0 1"; 
    Board board(fen);
    
    MoveGeneration moveGeneration(board);

    //std::vector<Move> moves = moveGeneration.generateMoves(WHITE);
    //printMoves(moves);

    u64 nodes = moveGeneration.perft(5, WHITE);

    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}