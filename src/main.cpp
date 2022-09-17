#include <iostream>
#include <string>
#include "board.hpp"
#include "moveGeneration.hpp"
#include "types.hpp"

int main(){
    Board board;
    std::string fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"; 
    board.initBoard(fen);
    
    MoveGeneration moveGeneration(board);
    Bitboard moves = moveGeneration.generateQueenMoves();
    std::cout << "Moves: " << moves << std::endl;
}