#include "../include/evaluation.hpp"
#include "../include/board.hpp"
#include "../include/functions.hpp"

Evaluation::Evaluation() {
    this->perspective = 0;
}

int Evaluation::evaluatePosition(Board board) {
    this->perspective = (board.sideToMove == WHITE) ? 1 : -1;

    Bitboard whiteKing = board.getKing(WHITE);
    Bitboard whiteQueens = board.getQueens(WHITE);
    Bitboard whiteRooks = board.getRooks(WHITE);
    Bitboard whiteBishops = board.getBishops(WHITE);
    Bitboard whiteKnights = board.getKnights(WHITE);
    Bitboard whitePawns = board.getPawns(WHITE);

    Bitboard blackKing = board.getKing(BLACK);
    Bitboard blackQueens = board.getQueens(BLACK);
    Bitboard blackRooks = board.getRooks(BLACK);
    Bitboard blackBishops = board.getBishops(BLACK);
    Bitboard blackKnights = board.getKnights(BLACK);
    Bitboard blackPawns = board.getPawns(BLACK);

    int materialScore = 
        KING_VALUE*(this->popcount(whiteKing) - this->popcount(blackKing)) + 
        QUEEN_VALUE*(this->popcount(whiteQueens) - this->popcount(blackQueens)) + 
        ROOK_VALUE*(this->popcount(whiteRooks) - this->popcount(blackRooks)) + 
        BISHOP_VAUE*(this->popcount(whiteBishops) - this->popcount(blackBishops)) + 
        KNIGHT_VALUE*(this->popcount(whiteKnights) - this->popcount(blackKnights)) + 
        PAWN_VALUE*(this->popcount(whitePawns) - this->popcount(blackPawns));

    int positionScore = //0; 
        calculatePositionScore(whiteKing, kingMiddlGamePositionEvaluationWhite) - calculatePositionScore(blackKing, kingMiddlGamePositionEvaluationBlack) +
        calculatePositionScore(whiteQueens, queenPositionEvaluationWhite) - calculatePositionScore(blackQueens, queenPositionEvaluationBlack) + 
        calculatePositionScore(whiteRooks, rookPositionEvaluationWhite) - calculatePositionScore(blackRooks, rookPositionEvaluationBlack) + 
        calculatePositionScore(whiteBishops, bishopPositionEvaluationWhite) - calculatePositionScore(blackBishops, bishopPositionEvaluationBlack) + 
        calculatePositionScore(whiteKnights, knightPositionEvaluationWhite) - calculatePositionScore(blackKnights, knightPositionEvaluationBlack) + 
        calculatePositionScore(whitePawns, pawnPositionEvaluationWhite) - calculatePositionScore(blackPawns, pawnPositionEvaluationBlack); 

    return (materialScore + positionScore) * this->perspective;
}

int Evaluation::calculatePositionScore(Bitboard pieces, const std::vector<int>& values) {
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