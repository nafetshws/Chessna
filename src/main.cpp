#include <iostream>
#include <string>
#include "board.hpp"
#include "moveGeneration.hpp"
#include "types.hpp"

int main(){
    Board board;
    std::string fen = "rnbqkbnr/ppp1pppp/8/8/3pP3/2N2N2/PPPP1PPP/R1BQKB1R b KQkq e3 0 3"; 
    board.initBoard();
    
    MoveGeneration moveGeneration(board);
    Bitboard moves = moveGeneration.generateAttackedSquares();
    std::cout << "Moves: " << moves << std::endl;
}