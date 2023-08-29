#include <iostream>

#include "../include/search.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/board.hpp"

Search::Search(Board board) {
    this->board = board;
    this->bestMove = Move(); 
    this->moveGeneration = MoveGeneration(this->board);
    this->sideToMove = WHITE;
}

int Search::alphaBeta(int alpha, int beta, int depth) {
    if(depth == 0) return 0; //return evaluation of the position

    std::vector<Move> moves = moveGeneration.generateMoves(this->sideToMove);
    for(Move move: moves) {
        Board copyBoard = this->board;
        
        int score = -this->alphaBeta(-beta, -alpha, depth-1);

        if(score >= beta) {
            return beta;
        }

        if(score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}
