#include <iostream>

#include "../include/search.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/board.hpp"
#include "../include/evaluation.hpp"

Search::Search(Board board) {
    this->board = board;
    this->bestMove = Move(); 
    this->moveGeneration = MoveGeneration(this->board);
    this->evaluation = Evaluation();
    this->sideToMove = WHITE;
}

int Search::alphaBeta(int alpha, int beta, int depth) {
    if(depth == 0) return this->evaluation.evaluatePosition(this->board); //return evaluation of the position

    std::vector<Move> moves = moveGeneration.generateMoves(this->sideToMove);
    for(Move move: moves) {
        Board copyBoard = this->board;
        this->board.makeMove(move); 
        int score = -this->alphaBeta(-beta, -alpha, depth-1);
        this->board = copyBoard;

        // Beta-cutoff: opponent will have chosen a different path down the tree as the move is too good
        if(score >= beta) {
            return beta;
        }

        //found new best move
        if(score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}
