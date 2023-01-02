#include <iostream>
#include <string>
#include "board.hpp"
#include "moveGeneration.hpp"
#include "types.hpp"

int main(){
    Board board;
    std::string fen = "r3k2r/pppppppp/2n1b3/3q1bn1/8/1NBQ1NP1/PPPPPPBP/R3K2R w KQkq - 0 1";
    board.initBoard();
    
    MoveGeneration moveGeneration(board);
    Bitboard moves = moveGeneration.generateAttackedSquares();

    //board.castleQueenSide(WHITE);
    board.prettyPrintBoard();
    
}