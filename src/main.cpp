#include <iostream>
#include <string>
#include "board.hpp"
#include "moveGeneration.hpp"
#include "types.hpp"

int main(){
    Board board;
    std::string fen = "rnbqkbnr/pppp1ppp/8/4p3/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; 
    board.initBoard(fen);
    
    MoveGeneration moveGeneration(board);
    Bitboard king = board.blackKing;
    Bitboard moves = moveGeneration.generateKingMoves(king);
    board.printBoard(moves);
}