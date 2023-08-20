#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    std::string fen = DEFAULT_FEN; 
    //std::string fen = "rnbq1bnr/ppppkppp/3Pp3/8/8/8/PPP1PPPP/RNBQKBNR b KQha - 0 1"; 
    Board board(fen);
    
    MoveGeneration moveGeneration(board);

    //std::vector<Move> moves = moveGeneration.generateMoves(WHITE);
    //printMoves(moves);

    u64 nodes = moveGeneration.perft(6, WHITE);

    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}