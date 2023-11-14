#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

#include "../include/types.hpp"
#include "../include/board.hpp"
#include "../include/search.hpp"

const float ENGINE_THINKING_TIME = 1;
const bool ENGINE_TINKING_TIME_LIMIT = false; 

class GameInterface {
    public:
        Board board;
        Search search;
        int lastMinDepth;
        static float maxTime;

        bool engingeThinkingTimeLimit; 
        float engineThinkingTime;

        //necessary for pondering
        float extraMoveTime;

        GameInterface();
        GameInterface(std::string fen);

        Move getBestEngineMove(bool ponder);
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