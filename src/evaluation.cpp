#include "../include/evaluation.hpp"
#include "../include/board.hpp"
#include "../include/functions.hpp"

Evaluation::Evaluation() {
    this->perspective = 0;
}

int Evaluation::evaluatePosition(Board board) {
    this->perspective = (board.sideToMove == WHITE) ? 1 : -1; 

    int materialScore = 
        KING_VALUE*(this->popcount(board.getKing(WHITE)) - this->popcount(board.getKing(BLACK))) + 
        QUEEN_VALUE*(this->popcount(board.getQueens(WHITE)) - this->popcount(board.getQueens(BLACK))) + 
        BISHOP_VAUE*(this->popcount(board.getBishops(WHITE)) - this->popcount(board.getBishops(BLACK))) + 
        KNIGHT_VALUE*(this->popcount(board.getKnights(WHITE)) - this->popcount(board.getKnights(BLACK))) + 
        PAWN_VALUE*(this->popcount(board.getPawns(WHITE)) - this->popcount(board.getPawns(BLACK)));

    int positionScore = 0; 
        //calculatePositionScore(board.getKing(WHITE), kingMiddlGamePositionEvaluation) - calculatePositionScore(board.getKing(BLACK), kingMiddlGamePositionEvaluation) +
        //calculatePositionScore(board.getQueens(WHITE), queenPositionEvaluation) - calculatePositionScore(board.getQueens(BLACK), queenPositionEvaluation) + 
        //calculatePositionScore(board.getRooks(WHITE), rookPositionEvaluation) - calculatePositionScore(board.getRooks(BLACK), rookPositionEvaluation) + 
        //calculatePositionScore(board.getBishops(WHITE), bishopPositionEvaluation) - calculatePositionScore(board.getBishops(BLACK), bishopPositionEvaluation) + 
        //calculatePositionScore(board.getKnights(WHITE), knightPositionEvaluation) - calculatePositionScore(board.getKnights(BLACK), knightPositionEvaluation) + 
        //calculatePositionScore(board.getPawns(WHITE), pawnPositionEvaluation) - calculatePositionScore(board.getPawns(BLACK), pawnPositionEvaluation); 

    //return (materialScore + positionScore) * this->sideToMove;
    return (materialScore + positionScore) * this->perspective;
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