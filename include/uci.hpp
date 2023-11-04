#ifndef UCI_H
#define UCI_H

#include <string>
#include "../include/board.hpp"
#include "../include/gameInterface.hpp"

/*
UCI (Universal Chess Interface) is necessary to communicate with GUIs.
It is necessary to communicate with lichess

Implementation based on: http://page.mi.fu-berlin.de/block/uci.htm
*/

const std::string ENGINE_NAME = "Chessna";
const int ENGINE_VERSION = 1; 
const std::string ENGINE_AUTHOR = "Stefan H";


class UCI {
    public:
        GameInterface gameInterface;

        float extraMoveTime;

        void processCommand(std::vector<std::string> args); 
        std::vector<std::string> convertInputToArgs(std::string input);
        void ponderhit();
    private:
        void initUCI();
        void isReady();
        void setOption(std::vector<std::string> args);
        void setUpBoard(std::vector<std::string> args);
        void searchBoard(std::vector<std::string> args);

        void makeMoveOnBoard(std::string move);

        void sendID();
        void sendOptions();
        void uciOK();
};

#endif