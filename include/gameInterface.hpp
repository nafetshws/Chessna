#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include "../include/types.hpp"
#include "../include/board.hpp"
#include "../include/search.hpp"

class GameInterface {
    public:
        Board board;

        GameInterface();
        GameInterface(std::string fen);

        Move getBestEngineMove();
        void playMove(std::string moveNotation);
        void play(Color playerColor);
    
    private:
        bool isPlaying;

};

#endif