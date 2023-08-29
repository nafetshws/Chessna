#ifndef SEARCH_H
#define SEARCH_H

#include "../include/types.hpp"
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/evaluation.hpp"

class Search {
    public:
        Board board;
        Move bestMove;
        MoveGeneration moveGeneration;
        Evaluation evaluation;
        Color sideToMove;

        Search(Board board);
        int alphaBeta(int alpha, int beta, int depth);

    private:
        const int positiveInfinity = 999999;
        const int negativeInfinity = -positiveInfinity;

};

#endif