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
    this->bestScore = 0;
    this->moveGeneration = MoveGeneration(this->board);
    this->evaluation = Evaluation();
    this->moveOrder = MoveOrder();
    this->sideToMove = WHITE;
    this->visitedNodes = 0;
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
    EvalFlag eFlag = HASH_ALPHA;
    //draw by 50 move rule
    if(this->board.halfMoveClock >= 100) return 0;

    //check if position was already evaluated and return according evaluation
    int ttEval = TranspositionTable::getTtEvaluation(this->board.zobristKey, depth, alpha, beta);
    if(ttEval != POS_NOT_FOUND) {
        if(depthFromRoot == 0) {
            this->bestMove = TranspositionTable::fetchEntry(this->board.zobristKey)->move;
            this->bestScore = ttEval;
        }

        return ttEval;
    }


    if(depth == 0) {
        this->visitedNodes++;
        int quiescenceEval = this->quiescenceSearch(alpha, beta);
        return quiescenceEval;
    }

    std::vector<Move> moves = moveGeneration.generateMoves(this->board, this->board.sideToMove, false);
    moveOrder.orderMoves(this->board, moves);

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
                this->bestMove = moves.at(i);
                this->bestScore = alpha;
            }
        }
    }
    TranspositionTable::storeTtEvaluation(this->board.zobristKey, depth, alpha, eFlag, bestMoveInPos);
    return alpha;
}

int Search::quiescenceSearch(int alpha, int beta) {
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
    moveOrder.orderMoves(this->board, captures); 

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
