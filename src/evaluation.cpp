#include "../include/evaluation.hpp"
#include "../include/board.hpp"
#include "../include/functions.hpp"

Evaluation::Evaluation() {
    this->sideToMove = 0;
}

int Evaluation::evaluatePosition(Board board) {
    Color c = board.sideToMove;
    Color oC = getOppositeColor(c);
    this->sideToMove = (c == WHITE) ? 1 : -1; //WHITE: 1; BLACK: -1

    int materialScore = 
        KING_VALUE*(this->popcount(board.getKing(c)) - this->popcount(board.getKing(oC))) + 
        QUEEN_VALUE*(this->popcount(board.getQueens(c)) - this->popcount(board.getQueens(oC))) + 
        BISHOP_VAUE*(this->popcount(board.getBishops(c)) - this->popcount(board.getBishops(oC))) + 
        KNIGHT_VALUE*(this->popcount(board.getKnights(c)) - this->popcount(board.getKnights(oC))) + 
        PAWN_VALUE*(this->popcount(board.getPawns(c)) - this->popcount(board.getPawns(oC)));

    return materialScore * this->sideToMove;
}

int Evaluation::popcount(Bitboard bitboard) {
    return std::__libcpp_popcount(bitboard);
}