#include <iostream>
#include <string>
#include "board.hpp"
#include "moveGeneration.hpp"
#include "types.hpp"

int main(){
    Board board;
    std::string fen = "rnbqkbnr/pppppppp/8/8/2B1P3/N2PBN2/PPP1QPPP/R3K2R w KQkq - 0 1";
    board.initBoard(fen);
    
    MoveGeneration moveGeneration(board);
    Bitboard moves = moveGeneration.generateKingMoves(CURRENT_POSITION, WHITE);

    std::cout << "Moves: " << moves << std::endl;

    
}