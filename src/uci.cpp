#include "../include/uci.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../include/functions.hpp"
#include "../include/zobrist.hpp"
#include "../include/transpositionTable.hpp"
#include "../include/repetitionTable.hpp"

void UCI::processCommand(std::vector<std::string> args) {
    //std::vector<std::string> args = this->convertInputToArgs(input);
    std::string command = args.at(0);

    if(command == "uci") {
        this->initUCI();
    } else if(command == "isready") {
        this->isReady();
    } else if(command == "setoption") {
        this->setOption(args);
    } else if(command == "position") {
        this->setUpBoard(args);
    } else if(command == "go") {
        this->searchBoard(args);
    } else if(command == "stop") {
        //need to implement thread
        std::cout << "recieved stop" << std::endl;
    } else if(command == "quit") {
        //stop programm
        exit(-1);
    } else if(command == "ucinewgame") {
        //Zobrist::init();
        //TranspositionTable::init(1024);
    }
}

std::vector<std::string> UCI::convertInputToArgs(std::string input) {
    std::vector<std::string> args;

    bool oneWord = true;
    int pos = 0;
    std::string delimiter = " ";
    std::string arg;

    while(input.find(delimiter) != std::string::npos) {
        pos = input.find(delimiter);
        arg = input.substr(0, pos);
        input.erase(0, pos + 1);
        args.push_back(arg);
        oneWord = false;
    }

    if(oneWord) args.push_back(input);

    return args;
}

void UCI::initUCI() {
    this->sendID();
    std::cout << std::endl;
    this->sendOptions();
    this->uciOK();
}

void UCI::isReady() {
    std::cout << "readyok" << std::endl;
}

void UCI::setOption(std::vector<std::string> args) {
    std::string optionName = args.at(2);
    //make option lowercase
    std::transform(optionName.begin(), optionName.end(), optionName.begin(), ::tolower);
    if(args.at(1) == "name" && optionName == "hash" && args.at(3) == "value") {
        int ttSize = std::stoi(args.at(4));
        if(ttSize >= 16 && ttSize <= 4096) {
            //set ttSize
            std::cout << "set tt size to: " << ttSize << std::endl;
        }
    }
}

void UCI::setUpBoard(std::vector<std::string> args) {
    //clear repetition table
    RepetitionTable::init();

    if(args.at(1) == "startpos") {
        this->gameInterface = GameInterface(DEFAULT_FEN);
        RepetitionTable::incrementPositionOccurence(this->gameInterface.board.zobristKey);

        //play moves
        if(args.size() >= 4 && args.at(2) == "moves") {
            for(int i = 3; i < args.size(); i++) {
                this->makeMoveOnBoard(args.at(i));
            }
        }
    } else {
        //fen has 6 args
        std::string fen = ""; 
        //position(1) + fen(6) + moves(1) + ...(?)
        if(args.size() >= 7) {
            for(int i = 1; i < 7; i++) {
                if(i == 6) fen += args.at(i);
                else fen += (args.at(i) + " "); 
            }
        }
        this->gameInterface = GameInterface(fen);
        RepetitionTable::incrementPositionOccurence(this->gameInterface.board.zobristKey);

        //play moves
        if(args.size() >= 9 && args.at(7) == "moves") {
            for(int i = 8; i < args.size(); i++) {
                this->makeMoveOnBoard(args.at(i));
            }
        }
    }
}

void UCI::searchBoard(std::vector<std::string> args) {
    //TODO::time management
    //GameInterface::maxTime = 3;
    
    float maxTimeForMove = 0.1f; 

    //check if movetime parameter was set
    if(std::find(args.begin(), args.end(), "movetime") != args.end()) {
        unsigned int moveTime = std::stoi(args.at(2)); 
        //take 90% of movetime to make move
        maxTimeForMove = (moveTime / 1000) * 0.9f;
    } else if(std::find(args.begin(), args.end(), "wtime") != args.end()) {
        float wtime = std::stoi(args.at(2)) / 1000;
        float btime = std::stoi(args.at(4)) / 1000;

        float engineMovetime = (this->gameInterface.board.sideToMove == WHITE) ? wtime : btime;  
        maxTimeForMove = engineMovetime / 35;

        //take increment into account
        if(std::find(args.begin(), args.end(), "winc") != args.end()) {
            float winc = std::stoi(args.at(6)) / 1000;
            float binc = std::stoi(args.at(8)) / 1000;

            float engineIncrement = (this->gameInterface.board.sideToMove == WHITE) ? winc : binc; 

            if(maxTimeForMove + (engineIncrement/2) < engineMovetime) {
                maxTimeForMove += (engineIncrement / 2);
            } else {
                maxTimeForMove = engineMovetime * 0.8;
            }
        }
    } else if(std::find(args.begin(), args.end(), "depth") != args.end()) {
        int depth = std::stoi(args.at(2));

        Move bestMove = this->gameInterface.getBestEngineMoveForDepth(depth);

        //print best move 
        std::cout << "bestmove " << printableMove(bestMove) << std::endl;
        return; 
    }

    GameInterface::maxTime = maxTimeForMove;

    Move bestMove = this->gameInterface.getBestEngineMove();

    //print best move 
    std::cout << "bestmove " << printableMove(bestMove) << std::endl;
}

void UCI::sendID() {
    //send engine name and version
    std::cout << "id name " << ENGINE_NAME << " " << ENGINE_VERSION << std::endl;
    //send author of engine 
    std::cout << "id author " << ENGINE_AUTHOR << std::endl;
}

void UCI::sendOptions() {
    //hash table size
    std::cout << "option name Hash type spin default 1024 min 16 max 4096" << std::endl;
    //NOTE: implement ponder
}

void UCI::uciOK() {
    std::cout << "uciok" << std::endl;
}

void UCI::makeMoveOnBoard(std::string move) {
    if(this->gameInterface.moveIsLegal(move)) {
        this->gameInterface.playMove(move);
    } else {
        this->gameInterface.board.prettyPrintBoard();
        std::cout << "Error: Invalid move (" << move << ")" << std::endl;
    }
}