#include <iostream>
#include <string>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"

int main(){
    std::string fen = "8/4k3/8/8/4R3/8/4K3/8 w - - 0 1";
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    Bitboard moves = moveGeneration.generateKingMoves(CURRENT_POSITION, BLACK);

    std::cout << "Moves: " << moves << std::endl;
}