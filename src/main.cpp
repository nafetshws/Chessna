#include <iostream>
#include <string>
#include "board.hpp"
#include "moveGeneration.hpp"
#include "types.hpp"

int main(){
    Board board;
    std::string fen = "rnb1kbnr/pppppppp/8/4Q3/4P3/8/PPPP1PPP/RNB1KBNR w KQkq - 0 1"; 
    board.initBoard(fen);
    
    MoveGeneration moveGeneration(board);
    Bitboard moves = moveGeneration.generateQueenMoves(board.whiteQueen, WHITE);
    std::cout << "Moves: " << moves << std::endl;
}