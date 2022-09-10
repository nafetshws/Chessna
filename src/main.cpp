#include <iostream>
#include <string>
#include "board.hpp"
#include "moveGeneration.hpp"

int main(){
    Board board;
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    board.initBoard(fen);
}