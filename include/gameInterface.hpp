#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include "../include/types.hpp"

class GameInterface {
    public:
        GameInterface();

        Move getBestEngineMove();
        void makeMove();
        void play();

};

#endif