#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <climits>

#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"
#include "../include/search.hpp"
#include "../include/gameInterface.hpp"
#include "../include/moveOrder.hpp"
#include "../include/transpositionTable.hpp"
#include "../include/zobrist.hpp"

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
    std::cout << "nodes/s: " << nodesPerS << std::endl;
}

void runSearch(std::string fen, int depth) {
    Board board(fen);

    Search search(board);

    u64 before = getCurrentTime(); 
    int eval = search.alphaBeta(negativeInfinity, positiveInfinity, depth, 0);
    u64 after = getCurrentTime(); 

    float dt = getTimeDifference(before, after);

    std::cout << "eval: " << search.bestScoreThisIteration << std::endl;
    std::cout << "best move: " << printableMove(search.bestMoveThisIteration) << std::endl;
    std::cout << "visited nodes: " << search.visitedNodes << std::endl;
    std::cout << "time (in s): " << dt << std::endl;
    std::cout << "nodes/s: " << (search.visitedNodes / dt) << std::endl;
}

void runIterativeDeepening(std::string fen, float timeInS) {
    Board board(fen);

    Search search(board);

    u64 before = getCurrentTime();
    search.iterativeDeepening(timeInS);
    u64 after = getCurrentTime();

    float dt = getTimeDifference(before, after);

    std::cout << "eval: " << search.bestScore << std::endl;
    std::cout << "best move: " << printableMove(search.bestMove) << std::endl;
    std::cout << "visited nodes: " << search.visitedNodes << std::endl;
    std::cout << "time (in s): " << dt << std::endl;
    std::cout << "min depth: " << search.minDepth << std::endl;
    std::cout << "nodes/s: " << (search.visitedNodes / dt) << std::endl;
}

void playAgainstEngine(std::string fen = DEFAULT_FEN) {
    GameInterface gameInterface(fen); 
    GameInterface::maxTime = 10;
    gameInterface.play(BLACK);
}



int main(int argc, char *argv[]){
    Zobrist::init();
    TranspositionTable::init(1000);

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
        depth = 10;
        //fen = "rnb1kbnr/p1qppppp/1pp5/8/3PP3/2N2N2/PPP2PPP/R1BQKB1R b KQkq - 0 1"; 
        fen = "6k1/5p2/6p1/8/7p/8/6PP/6K1 b - - 0 1";
    }

    //runMoveGeneration(fen, depth);
    //runSearch(fen, depth);

    //playAgainstEngine();

    //depth = time in seconds
    runIterativeDeepening(fen, depth);
    //runIterativeDeepening("r1bqkb1r/pppppppp/2n2n2/8/4P3/2N2N2/PPPP1PPP/R1BQKB1R b KQkq - 4 3", 5);

    //Board board("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1");
    //MoveGeneration mg(board);

    //printMoves(mg.generateMoves(board, BLACK));

    return 0;
}