#ifndef SEARCH_H
#define SEARCH_H

#include "../include/types.hpp"
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/evaluation.hpp"
#include "../include/moveOrder.hpp"

const int positiveInfinity = 999999;
const int negativeInfinity = -positiveInfinity;

const Move EMPTY_MOVE = Move();

class Search {
    public:
        Board board;
        Move bestMove;
        Move bestMoveThisIteration;
        Move prevBestMove;
        int bestScore;
        int bestScoreThisIteration;
        int prevBestScore;
        MoveGeneration moveGeneration;
        Evaluation evaluation;
        MoveOrder moveOrder;
        Color sideToMove;

        bool searchIsCancelled;
        u64 startTime;
        float maxSearchTime;
        
        bool isTimeLimit;

        int minDepth;
        u64 visitedNodes;

        Search(Board board);

        void iterativeDeepening(float timeInS);

        int alphaBeta(int alpha, int beta, int depth, int depthFromRoot);
        int quiescenceSearch(int alpha, int beta, int depthFromRoot);
        int negaMax(int depth, int depthFromRoot);

        void cancelSearch();
        bool getSearchIsCancelled();
        void checkTimeLimit();

};

#endif