#include <iostream>
#include <string>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"

int main(){
    //std::string fen = "rnbqkbnr/pppppppp/8/8/2B1P3/N2PBN2/PPP1QPPP/R3K2R w KQkq - 0 1";
    Board board;
    
    MoveGeneration moveGeneration{board};
    //Bitboard moves = moveGeneration.generateKingMoves(CURRENT_POSITION, WHITE);
    Bitboard aSquares = moveGeneration.generateAttackedSquares(WHITE);

    std::cout << "Moves: " << aSquares << std::endl;
}