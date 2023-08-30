#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"
#include "../include/search.hpp"

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
        depth = 2;
        fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/P1N2Q2/1PPBBPpP/2KR3R b kq - 0 1"; 
    }


    //Board board(fen);
    //MoveGeneration moveGeneration(board);

    //Color color = board.sideToMove;

    //std::vector<Move> moves = moveGeneration.generateMoves(WHITE);
    //printMoves(moves);

    //u64 nodes = moveGeneration.perft(depth, color);
    //std::cout << "Nodes: " << nodes << std::endl;

    Board board(DEFAULT_FEN);

    Search search(board);

    int eval = search.alphaBeta(negativeInfinity, positiveInfinity, 6, 0);
    std::cout << "eval: " << eval << std::endl;
    std::cout << "best move: " << printableMove(search.bestMove) << std::endl;
    std::cout << "visited nodes: " << search.visitedNodes << std::endl;


    return 0;
}