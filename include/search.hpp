#ifndef SEARCH_H
#define SEARCH_H

#include "../include/types.hpp"
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/evaluation.hpp"

const int positiveInfinity = 999999;
const int negativeInfinity = -positiveInfinity;

class Search {
    public:
        Board board;
        Move bestMove;
        int bestScore;
        MoveGeneration moveGeneration;
        Evaluation evaluation;
        Color sideToMove;

        u64 visitedNodes;

        Search(Board board);
        int alphaBeta(int alpha, int beta, int depth, int depthFromRoot);

};

#endif