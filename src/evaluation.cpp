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

    int positionScore = 0; 
        calculatePositionScore(board.getKing(c), kingMiddlGamePositionEvaluation) - calculatePositionScore(board.getKing(oC), kingMiddlGamePositionEvaluation) +
        calculatePositionScore(board.getQueens(c), queenPositionEvaluation) - calculatePositionScore(board.getQueens(oC), queenPositionEvaluation) + 
        calculatePositionScore(board.getRooks(c), rookPositionEvaluation) - calculatePositionScore(board.getRooks(oC), rookPositionEvaluation) + 
        calculatePositionScore(board.getBishops(c), bishopPositionEvaluation) - calculatePositionScore(board.getBishops(oC), bishopPositionEvaluation) + 
        calculatePositionScore(board.getKnights(c), knightPositionEvaluation) - calculatePositionScore(board.getKnights(oC), knightPositionEvaluation) + 
        calculatePositionScore(board.getPawns(c), pawnPositionEvaluation) - calculatePositionScore(board.getPawns(oC), pawnPositionEvaluation); 

    return (materialScore + positionScore) * this->sideToMove;
}

int Evaluation::calculatePositionScore(Bitboard pieces, std::vector<int> values) {
    int sum = 0;
    while(pieces != 0) {
        int index = __builtin_ctzll(pieces);

        sum += values.at(index);

        pieces &= ~squareToBitboard(index);
    }

    return sum;
}

int Evaluation::popcount(Bitboard bitboard) {
    return std::__libcpp_popcount(bitboard);
}