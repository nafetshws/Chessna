#include <iostream>
#include <string>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"

int main(){
    std::string fen = "8/4k3/8/2B3Q1/8/8/8/8 w - - 0 1";
    Board board(fen);
    
    MoveGeneration moveGeneration(board);
    int inCheck = moveGeneration.isInCheck();

    std::cout << "is in check: " << inCheck << std::endl;

    return 0;
}