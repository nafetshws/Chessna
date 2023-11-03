#include <iostream>

#include "../include/search.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/board.hpp"
#include "../include/evaluation.hpp"
#include "../include/functions.hpp"
#include "../include/transpositionTable.hpp"
#include "../include/repetitionTable.hpp"


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
    this->isTimeLimit = false;
    this->isDebugMode = true;
}

void Search::startIterativeDeepening(int depth) {
    this->searchIsCancelled = false;
    this->minDepth = 0;
    this->visitedNodes = 0;
    this->maxDepth = depth;
    this->isTimeLimit = false;
    this->isDebugMode = true;

    this->iterativeDeepening();
}

void Search::startIterativeDeepening(float timeInS) {
    this->searchIsCancelled = false;
    this->startTime = getCurrentTime();
    this->maxSearchTime = timeInS;
    this->minDepth = 0;
    this->visitedNodes = 0;
    this->isTimeLimit = true;
    this->isDebugMode = true;

    this->iterativeDeepening();
}

void Search::iterativeDeepening() {
    for(int currentSearchDepth = 1; currentSearchDepth < 200; currentSearchDepth++) {
        checkTimeLimit();

        //necessary for debug info
        this->visitedNodes = 0;
        this->iterationStartTime = getCurrentTime();

        //time in s can be interpreted as depth
        if(!this->isTimeLimit && (currentSearchDepth > this->maxDepth)) this->cancelSearch();

        if(this->getSearchIsCancelled()) break;

        int eval = alphaBeta(negativeInfinity, positiveInfinity, currentSearchDepth, 0);
        
        if(this->getSearchIsCancelled()) {
            this->bestMove = bestMoveThisIteration;
            this->bestScore = bestScoreThisIteration;
        } else {
            //save results of this iteration
            this->bestMove = this->bestMoveThisIteration;
            this->prevBestMove = this->bestMoveThisIteration;

            this->bestScore = this->bestScoreThisIteration;
            this->prevBestScore = this->bestScoreThisIteration;
            this->minDepth++;
        }
        //send info
        if(this->isDebugMode) this->sendDebugInfo();
    }
}

int Search::alphaBeta(int alpha, int beta, int depth, int depthFromRoot) {
    checkTimeLimit();

    if(this->getSearchIsCancelled()) {
        return 0;
    }

    EvalFlag eFlag = HASH_ALPHA;
    //draw by 50 move rule
    if(this->board.halfMoveClock >= 100) return 0;

    //check for 3-fold repetition
    if(depthFromRoot > 0 && RepetitionTable::checkForDrawByRepetition(this->board.zobristKey)) return 0;

    //check if position was already evaluated and return according evaluation
    int ttEval = TranspositionTable::getTtEvaluation(this->board.zobristKey, depth, alpha, beta, depthFromRoot);
    if(ttEval != POS_NOT_FOUND && ttEval != POS_NOT_DEEP_ENOUGH) {
        if(depthFromRoot == 0) {
            this->bestMoveThisIteration = TranspositionTable::fetchEntry(this->board.zobristKey)->move;
            this->bestScoreThisIteration = ttEval;
        }
        return ttEval;
    }

    if(depth == 0) {
        this->visitedNodes++;
        return this->quiescenceSearch(alpha, beta, depthFromRoot+1);
    }

    //prev best move is always from
    Move possibleBestMove = EMPTY_MOVE; 
    if(depthFromRoot == 0) {
        possibleBestMove = this->prevBestMove;
    } else if(ttEval == POS_NOT_DEEP_ENOUGH) {
        possibleBestMove = TranspositionTable::fetchEntry(this->board.zobristKey)->move; 
    }

    std::vector<Move> moves = moveGeneration.generateMoves(this->board, this->board.sideToMove, false);
    moveOrder.orderMoves(this->board, moves, possibleBestMove, depthFromRoot);

    if(moves.size() == 0) {
        if(moveGeneration.check_info.numberOfChecks != 0) {
            //checkmate
            int mateScore = INSTANT_MATE - depthFromRoot;
            return -mateScore; 
        } else {
            //stalemate
            return 0;
        }
        return 0;
    }

    Move bestMoveInPos = Move(); //creates NULL move

    for(int i = 0; i < moves.size(); i++) {
        //copy board to undo move
        Board copyBoard = this->board;

        this->board.makeMove(moves.at(i)); 

        int score = 0;
        bool fullSearch = true;

        /*
        LMR (Late Move Reductions)
        Assuming move ordering does a fine job, later moves in the vector will probably be worse
        and therefore not be worth searching them to the same depth. Though if the search with a lower
        depth returns an unexpectedly high eval, another full search is done. 
        */
        if(i >= 3 && depth >= 3 && moves.at(i).moveType != CAPTURE) {
            score = -this->alphaBeta(-alpha-1, -alpha, depth-2, depthFromRoot + 1);
            if(score <= alpha) fullSearch = false; 
        }

        if(fullSearch) score = -this->alphaBeta(-beta, -alpha, depth-1, depthFromRoot + 1);
        this->board = copyBoard;

        if(this->getSearchIsCancelled()) {
            return 0;
        }

        // Beta-cutoff: opponent will have chosen a different path down the tree as the move is too good
        if(score >= beta) {
            //record killer move
            if(moves.at(i).moveType == QUIET) {
                this->moveOrder.insertKillerMove(Killermove(moves.at(i), depthFromRoot, score), depthFromRoot);
            }

            TranspositionTable::storeTtEvaluation(this->board.zobristKey, depth, beta, HASH_BETA, moves.at(i), depthFromRoot);
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

    if(printableMove(bestMoveInPos) != "errorerror") TranspositionTable::storeTtEvaluation(this->board.zobristKey, depth, alpha, eFlag, bestMoveInPos, depthFromRoot);
    return alpha;
}

int Search::quiescenceSearch(int alpha, int beta, int depthFromRoot) {
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
    moveOrder.orderMoves(this->board, captures, EMPTY_MOVE, depthFromRoot); 

    for(int i = 0; i < captures.size(); i++) {
        Board copyBoard = this->board;
        this->board.makeMove(captures.at(i));
        int score = -this->quiescenceSearch(-beta, -alpha, depthFromRoot+1);

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
    if(this->isTimeLimit && getTimeDifference(this-> startTime, getCurrentTime()) >= this->maxSearchTime) this->cancelSearch(); 
}


void Search::sendDebugInfo() {
    int nps = this->visitedNodes / getTimeDifference(this->iterationStartTime, getCurrentTime()); 
    std::cout << "info depth " << this->minDepth << " score cp " << this->bestScoreThisIteration << " nodes " << this->visitedNodes << " nps " <<nps << " pv " << printableMove(this->bestMoveThisIteration) << std::endl;
}