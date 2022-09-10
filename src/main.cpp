#include <iostream>
#include <string>
#include "board.hpp"
#include "moveGeneration.hpp"
#include "types.hpp"

int main(){
    Board board;
    std::string fen = "r1bqkb1r/pppppppp/8/5n2/2n5/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; 
    board.initBoard(fen);
    
    MoveGeneration moveGeneration(board);
    Bitboard moves = moveGeneration.generateKnightMoves();
    board.printBoard(moves);
}