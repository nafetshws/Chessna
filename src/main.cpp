#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(){
    std::string fen = "r1b1k2r/ppp2ppp/3p4/4Q3/8/3P4/PPP2PPP/RNBQKBNR w KQkq - 0 1";
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    std::vector<Move> moves = moveGeneration.generateMoves(BLACK);

    std::cout << "moves: " << moves.size() << std::endl;

    printMoves(moves);


    return 0;
}