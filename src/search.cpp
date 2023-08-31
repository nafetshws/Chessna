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

int Search::negaMax(int depth) {
    if(depth == 0) {
        this->visitedNodes++;
        return this->evaluation.evaluatePosition(this->moveGeneration.board);
    }

    std::vector<Move> moves = moveGeneration.generateMoves(this->moveGeneration.board.sideToMove);

    int maxScore = negativeInfinity;

    for(int i = 0; i < moves.size(); i++) {
        Board copyBoard = this->moveGeneration.board;
        this->moveGeneration.board.makeMove(moves.at(i));
        int score = -this->negaMax(depth-1);
        this->moveGeneration.board = copyBoard;

        if(score > maxScore) {
            maxScore = score;
        }
    }
    return maxScore;
}

int Search::alphaBeta(int alpha, int beta, int depth, int depthFromRoot) {
    if(depth == 0) return this->evaluation.evaluatePosition(this->board); //return evaluation of the position

    std::vector<Move> moves = moveGeneration.generateMoves(this->board.sideToMove);
    for(int i = 0; i < moves.size(); i++) {
        this->visitedNodes++;
        //copy board to undo move
        Board copyBoard = this->board;

        this->board.makeMove(moves.at(i)); 
        int score = -this->alphaBeta(-beta, -alpha, depth-1, depthFromRoot + 1);
        this->board = copyBoard;

        // Beta-cutoff: opponent will have chosen a different path down the tree as the move is too good
        //if(score >= beta) {
        //    return beta;
        //}

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
