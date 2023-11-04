#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <climits>
#include <sstream>
#include <thread>

#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"
#include "../include/search.hpp"
#include "../include/gameInterface.hpp"
#include "../include/moveOrder.hpp"
#include "../include/transpositionTable.hpp"
#include "../include/zobrist.hpp"
#include "../include/uci.hpp"
#include "../include/repetitionTable.hpp"

bool searchHasEnded = false;;

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
    search.startIterativeDeepening(timeInS);
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

std::vector<std::string> readCin() {
    std::string inputLine;
    std::vector<std::string> args;
    std::getline(std::cin, inputLine);
    std::istringstream iss(inputLine);

    std::string arg;

    while(iss >> arg) {
        args.push_back(arg);
    }

    return args;
}

void listenForStopSearch(UCI *uci) {
    while(!searchHasEnded) {
        std::vector<std::string> input = readCin();
        std::cout << input.at(0) << std::endl;
        if(input.size() == 1 && input.at(0) == "stop") {
            //stop search
            uci->gameInterface.endSearch();
            std::terminate();
            searchHasEnded = true;
            return;
        } else if(input.size() == 1 && input.at(0) == "ponderhit") {
            uci->ponderhit();
            //uci->gameInterface.endSearch();
            //searchHasEnded = true;
            //return;
        }
        //sleep for 100ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void runUCI() {
    UCI uci;

    std::string command = "";

    while(command != "quit") {
        std::vector<std::string> args = readCin();
        command = args.at(0);;

        //start thread to listen for stop
        if(command == "go") {
            searchHasEnded = false;
            std::thread endSearchWorker(listenForStopSearch, &uci);

            uci.processCommand(args);
            searchHasEnded = true;

            endSearchWorker.join();
            endSearchWorker.~thread();
        } else {
            uci.processCommand(args);
        }
        
    }
}



int main(int argc, char *argv[]){
    Zobrist::init();
    TranspositionTable::init();
    RepetitionTable::init();

    runUCI();

    return 0;
}