#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include "../include/types.hpp"
#include "../include/board.hpp"
#include "../include/search.hpp"

class GameInterface {
    public:
        Board board;
        Search search;
        int lastMinDepth;
        static float maxTime;

        GameInterface();
        GameInterface(std::string fen);

        Move getBestEngineMove();
        Move getBestEngineMoveForDepth(int depth);
        void playMove(std::string moveNotation);
        void play(Color playerColor);
        bool moveIsLegal(std::string moveNotation); 
        GameStatus getGameStatus(); 
        void endSearch();
    
    private:
        bool isPlaying;

};

#endif