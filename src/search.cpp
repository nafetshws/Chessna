#include <iostream>

#include "../include/search.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/board.hpp"
#include "../include/evaluation.hpp"

Search::Search(Board board) {
    this->board = board;
    this->bestMove = Move(); 
    this->bestScore = 0;
    this->moveGeneration = MoveGeneration(this->board);
    this->evaluation = Evaluation();
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
    if(depth == 0) {
        this->visitedNodes++;
        //return this->evaluation.evaluatePosition(this->board);
        return this->quiescenceSearch(alpha, beta);//this->evaluation.evaluatePosition(this->board); //return evaluation of the position
    }

    std::vector<Move> moves = moveGeneration.generateMoves(this->board, this->board.sideToMove, false);
    for(int i = 0; i < moves.size(); i++) {
        //copy board to undo move
        Board copyBoard = this->board;

        this->board.makeMove(moves.at(i)); 
        int score = -this->alphaBeta(-beta, -alpha, depth-1, depthFromRoot + 1);
        this->board = copyBoard;

        // Beta-cutoff: opponent will have chosen a different path down the tree as the move is too good
        if(score >= beta) {
            return beta;
        }

        //found new best move
        if(score > alpha) {
            alpha = score;

            if(depthFromRoot == 0) {
                this->bestMove = moves.at(i);
                this->bestScore = alpha;
            }
        }
    }

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
