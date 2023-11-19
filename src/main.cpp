#include <iostream>
#include <string>
#include <vector>

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

void playAgainstEngine(std::string fen = DEFAULT_FEN) {
    GameInterface gameInterface(fen); 
    GameInterface::maxTime = 10;
    gameInterface.play(BLACK);
}


int main(int argc, char *argv[]){
    Zobrist::init();
    TranspositionTable::init();
    RepetitionTable::init();

	UCI uci;
    uci.runUCI();

    return 0;
}
