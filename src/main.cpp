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
    search.alphaBeta(negativeInfinity, positiveInfinity, depth, 0);
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

void runUCI() {
    UCI uci;

    std::string command = "";

    std::thread worker1;
    bool isProcessing = false;
    bool threadIsInitialized = false;

    /*
    Main thread is continuously checking for user input.
    If it receives a "go" command it starts a seperate thread that handles the calculation.
    Otherwise it exectues the command in the main thread.
    */

    while(command != "quit") { 
        std::vector<std::string> args = readCin();
        command = args.at(0);;

        if(command == "stop") {
            uci.gameInterface.endSearch();
            worker1.join(); //wait for thread to finish calculation (shouldn't take long)
            isProcessing = false; //calc is done
            threadIsInitialized = false; //thread is finished -> it isn't initialized anymore
        } else if(command == "ponderhit") {
            uci.ponderhit();
            worker1.join(); //wait for thread to finish
            isProcessing = false; //calc is done
            threadIsInitialized = false; //thread isn't initialized anymore
        }

        if(!isProcessing && command == "go") {
            if(threadIsInitialized) worker1.join(); //wait for thread to finish if it didn't receive the stop/ponderhit command but finished calculating
            worker1 = std::thread(&UCI::processCommand, &uci, args, &isProcessing); //start new thread
            isProcessing = true;
            threadIsInitialized = true;
        } else if(!isProcessing) {
            uci.processCommand(args, &isProcessing); //handle other commands like "position" etc.
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
