#include <iostream>
#include <string>
#include "board.hpp"
#include "moveGeneration.hpp"
#include "types.hpp"

int main(){
    Board board;
    std::string fen = "r1bqk1nr/pppp1ppp/2n5/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 1"; 
    board.initBoard(fen);
    
    MoveGeneration moveGeneration(board);
    Bitboard moves = moveGeneration.generateKingMoves(board.whiteKing, WHITE);
    moveGeneration.castleKingSide(WHITE);
    std::cout << "Moves: " << moves << std::endl;

    std::cout << "Occupied: " << moveGeneration.board.getOccupied() << std::endl;
    std::cout << "rooks: " << moveGeneration.board.whiteRooks << std::endl;
    std::cout << "king: " << moveGeneration.board.whiteKing << std::endl;
}