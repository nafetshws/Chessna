#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

int main(int argc, char *argv[]){
    std::string fen; 
    int depth; 

    if(argc == 3) {
        std::string input = argv[1];
        if(input.compare("default") == 0) {
            fen = DEFAULT_FEN;
        } else {
            fen = argv[1];
        }
        depth = std::stoi(argv[2]);
    } else {
        depth = 1;
        fen = "r3k2r/p1p1qpb1/bn1ppnp1/1B1PN3/1p2P3/2N2Q1p/PPPB1PPP/R4K1R b kq - 0 1"; 
    }
    Board board(fen);
    MoveGeneration moveGeneration(board);

    Color color = board.sideToMove;

    //std::vector<Move> moves = moveGeneration.generateMoves(WHITE);
    //printMoves(moves);

    u64 nodes = moveGeneration.perft(depth, color);
    std::cout << "Nodes: " << nodes << std::endl;


    return 0;
}