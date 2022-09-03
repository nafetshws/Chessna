#include <iostream>
#include "MoveGeneration.hpp"
#include "board.hpp"

int main(){
    Board board;
    board.initBoard();
    MoveGeneration moveGeneration(board);
    Bitboard res = moveGeneration.Southwest(63);
    std::cout << res << std::endl;
}