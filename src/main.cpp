#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"
#include "../include/search.hpp"
#include <chrono>

void runMoveGeneration(std::string fen, int depth) {
    Board board(fen);
    MoveGeneration moveGeneration(board);

    Color color = board.sideToMove;

    u64 before = getCurrentTime();
    u64 nodes = moveGeneration.perft(depth, color);
    u64 after = getCurrentTime();

    float nodesPerS = nodes/getTimeDifference(before, after);

    std::cout << "Nodes: " << nodes << std::endl;
    std::cout << "attack: " << moveGeneration.underAttackCounter << std::endl;
    std::cout << "time: " << getTimeDifference(before, after) << std::endl;
    std::cout << "nodes/s: " << nodesPerS<< std::endl;
}

void runSearch(std::string fen, int depth) {
    Board board(fen);

    Search search(board);

    u64 before = getCurrentTime(); 
    int eval = search.alphaBeta(negativeInfinity, positiveInfinity, depth, 0);
    //int eval = search.negaMax(depth);
    u64 after = getCurrentTime(); 
    float dt = getTimeDifference(before, after);
    std::cout << "eval: " << eval << std::endl;
    std::cout << "best move: " << printableMove(search.bestMove) << std::endl;
    std::cout << "visited nodes: " << search.visitedNodes << std::endl;
    std::cout << "time (in s): " << dt << std::endl;
    std::cout << "nodes/s: " << (search.visitedNodes / dt) << std::endl;
}

int main(int argc, char *argv[]){
    std::string fen; 
    int depth; 

    if(argc == 3) {
        std::string input = argv[1];
        if(input.compare("default") == 0) {
            fen = DEFAULT_FEN;
        } else if(input.compare("middle") == 0) {
            fen = "r3k2r/pppq1pp1/2np1n1p/2b1p1B1/2B1P1b1/2NP1N1P/PPPQ1PP1/R3K2R w KQkq - 0 1";
        } else {
            fen = argv[1];
        }
        depth = std::stoi(argv[2]);
    } else {
        depth = 2;
        fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/P1N2Q2/1PPBBPpP/2KR3R b kq - 0 1"; 
    }

    runMoveGeneration(fen, depth);
    //runSearch(fen, depth);

    return 0;
}