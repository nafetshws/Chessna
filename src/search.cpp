#include <iostream>

#include "../include/search.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/board.hpp"
#include "../include/evaluation.hpp"
#include "../include/functions.hpp"
#include "../include/transpositionTable.hpp"
#include "../include/transpositionTable.hpp"

Search::Search(Board board) {
    this->board = board;
    this->bestMove = Move(); 
    this->bestMoveThisIteration = Move(); 
    this->prevBestMove = Move(); 
    this->bestScore = 0;
    this->bestScoreThisIteration = 0;
    this->prevBestScore = 0;
    this->moveGeneration = MoveGeneration(this->board);
    this->evaluation = Evaluation();
    this->moveOrder = MoveOrder();
    this->sideToMove = WHITE;
    this->visitedNodes = 0;
    this->searchIsCancelled = false;
    this->startTime = 0;
    this->maxSearchTime = 0;
    this->minDepth = 0;
}

void Search::iterativeDeepening(float timeInS) {
    this->searchIsCancelled = false;
    this->startTime = getCurrentTime();
    this->maxSearchTime = timeInS;
    this->minDepth = 0;

    for(int currentSearchDepth = 1; currentSearchDepth < 100; currentSearchDepth++) {
        //if(getTimeDifference(this-> startTime, getCurrentTime()) >= timeInS) this->cancelSearch(); 
        checkTimeLimit();

        if(this->getSearchIsCancelled()) break;

        int eval = alphaBeta(negativeInfinity, positiveInfinity, currentSearchDepth, 0);

        if(this->getSearchIsCancelled()) {
            this->bestMove = prevBestMove;
            this->bestScore = prevBestScore;
        } else {
            //save results of this iteration
            this->bestMove = this->bestMoveThisIteration;
            this->prevBestMove = this->bestMoveThisIteration;

            this->bestScore = this->bestScoreThisIteration;
            this->prevBestScore = this->bestScoreThisIteration;
            this->minDepth++;
        }

    }
}

int Search::negaMax(int depth, int depthFromRoot) {
    if(depth == 0) {
        this->visitedNodes++;
        return this->evaluation.evaluatePosition(this->board);
    }

    std::vector<Move> moves = moveGeneration.generateMoves(this->board, this->board.sideToMove, false);

    int maxScore = negativeInfinity;

    for(int i = 0; i < moves.size(); i++) {
        Board copyBoard = this->board;
        this->board.makeMove(moves.at(i));
        int score = -this->negaMax(depth-1, depthFromRoot+1);
        this->board = copyBoard;

        if(score > maxScore) {
            maxScore = score;

            if(depthFromRoot == 0) {
                this->bestMove = moves.at(i);
            }
        }
    }
    return maxScore;
}

int Search::alphaBeta(int alpha, int beta, int depth, int depthFromRoot) {
    //NOTE: should be implemented in another thread
    checkTimeLimit();

    if(this->getSearchIsCancelled()) {
        return 0;
    }

    EvalFlag eFlag = HASH_ALPHA;
    //draw by 50 move rule
    if(this->board.halfMoveClock >= 100) return 0;

    //check if position was already evaluated and return according evaluation
    int ttEval = TranspositionTable::getTtEvaluation(this->board.zobristKey, depth, alpha, beta);
    if(ttEval != POS_NOT_FOUND && ttEval != POS_NOT_DEEP_ENOUGH) {
        if(depthFromRoot == 0) {
            this->bestMoveThisIteration = TranspositionTable::fetchEntry(this->board.zobristKey)->move;
            this->bestScoreThisIteration = ttEval;
        }

        return ttEval;
    }


    if(depth == 0) {
        this->visitedNodes++;
        return this->quiescenceSearch(alpha, beta);
    }

    //prev best move is always from
    Move possibleBestMove = EMPTY_MOVE; 
    if(depthFromRoot == 0) {
        possibleBestMove = this->prevBestMove;
    } else if(ttEval == POS_NOT_DEEP_ENOUGH) {
        possibleBestMove = TranspositionTable::fetchEntry(this->board.zobristKey)->move; 
    }

    std::vector<Move> moves = moveGeneration.generateMoves(this->board, this->board.sideToMove, false);
    moveOrder.orderMoves(this->board, moves, possibleBestMove);

    if(moves.size() == 0) {
        if(moveGeneration.check_info.numberOfChecks != 0) {
            //checkmate
            int mateScore = INSTANT_MATE - depthFromRoot;
            return -mateScore; 
        } else {
            //stalemate
            return 0;
        }
    }

    Move bestMoveInPos = Move(); //creates NULL move

    for(int i = 0; i < moves.size(); i++) {
        //copy board to undo move
        Board copyBoard = this->board;

        this->board.makeMove(moves.at(i)); 
        int score = -this->alphaBeta(-beta, -alpha, depth-1, depthFromRoot + 1);
        this->board = copyBoard;

        if(this->getSearchIsCancelled()) {
            return 0;
        }

        // Beta-cutoff: opponent will have chosen a different path down the tree as the move is too good
        if(score >= beta) {
            TranspositionTable::storeTtEvaluation(this->board.zobristKey, depth, beta, HASH_BETA, moves.at(i));
            return beta;
        }

        //found new best move
        if(score > alpha) {
            alpha = score;
            bestMoveInPos = moves.at(i);

            eFlag = HASH_EXACT;

            if(depthFromRoot == 0) {
                this->bestMoveThisIteration = moves.at(i);
                this->bestScoreThisIteration = alpha;
            }
        }
    }

    TranspositionTable::storeTtEvaluation(this->board.zobristKey, depth, alpha, eFlag, bestMoveInPos);
    return alpha;
}

int Search::quiescenceSearch(int alpha, int beta) {
    if(this->getSearchIsCancelled()) {
        return 0;
    }    

    int eval = this->evaluation.evaluatePosition(this->board);
    this->visitedNodes++;
    //non-capture moves can be better than bad captures. It might not be necessary to continue search
    //when there are good non-capture alternatives
    if(eval >= beta) {
        return beta;
    }

    if(eval > alpha) {
        alpha = eval;
    }

    std::vector<Move> captures = this->moveGeneration.generateMoves(this->board, this->board.sideToMove, true);
    moveOrder.orderMoves(this->board, captures, EMPTY_MOVE); 

    for(int i = 0; i < captures.size(); i++) {
        Board copyBoard = this->board;
        this->board.makeMove(captures.at(i));
        int score = -this->quiescenceSearch(-beta, -alpha);
        this->board = copyBoard;

        //beta-cutoff
        if(score >= beta) {
            return beta;
        }
        if(score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}

void Search::cancelSearch() {
    this->searchIsCancelled = true;
}

bool Search::getSearchIsCancelled() {
    return this->searchIsCancelled;
}

void Search::checkTimeLimit() {
    if(getTimeDifference(this-> startTime, getCurrentTime()) >= this->maxSearchTime) this->cancelSearch(); 
}
