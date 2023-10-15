#include "../include/evaluation.hpp"
#include "../include/board.hpp"
#include "../include/functions.hpp"
#include <iostream>
#include <cmath>

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

    int materialScoreWhite = 
        KING_VALUE * this->popcount(whiteKing) + 
        QUEEN_VALUE * this->popcount(whiteQueens) +  
        ROOK_VALUE * this->popcount(whiteRooks) +
        BISHOP_VAUE * this->popcount(whiteBishops) + 
        KNIGHT_VALUE * this->popcount(whiteKnights) + 
        PAWN_VALUE * this->popcount(whitePawns);

    int materialScoreBlack = 
        KING_VALUE * this->popcount(blackKing) + 
        QUEEN_VALUE * this->popcount(blackQueens) +  
        ROOK_VALUE * this->popcount(blackRooks) +
        BISHOP_VAUE * this->popcount(blackBishops) + 
        KNIGHT_VALUE * this->popcount(blackKnights) + 
        PAWN_VALUE * this->popcount(blackPawns);

    int materialScore = materialScoreWhite - materialScoreBlack;


    //check if endgame
    bool isEndgame = false;
    int endgameSum =  materialScoreWhite - KING_VALUE*this->popcount(whiteKing) - PAWN_VALUE*this->popcount(whitePawns); 
    if(endgameSum <= 1000) {
        isEndgame = true;

    }

    int positionScore = 
        calculatePositionScore(whiteKing, (isEndgame) ? kingEndGamePositionEvaluationWhite: kingMiddlGamePositionEvaluationWhite) - calculatePositionScore(blackKing, (isEndgame) ? kingEndGamePositionEvaluationBlack : kingMiddlGamePositionEvaluationBlack) +
        calculatePositionScore(whiteQueens, queenPositionEvaluationWhite) - calculatePositionScore(blackQueens, queenPositionEvaluationBlack) + 
        calculatePositionScore(whiteRooks, rookPositionEvaluationWhite) - calculatePositionScore(blackRooks, rookPositionEvaluationBlack) + 
        calculatePositionScore(whiteBishops, bishopPositionEvaluationWhite) - calculatePositionScore(blackBishops, bishopPositionEvaluationBlack) + 
        calculatePositionScore(whiteKnights, knightPositionEvaluationWhite) - calculatePositionScore(blackKnights, knightPositionEvaluationBlack) + 
        calculatePositionScore(whitePawns, pawnPositionEvaluationWhite) - calculatePositionScore(blackPawns, pawnPositionEvaluationBlack); 

    //mopup evaluation
    int mopUpEval = 0;
    if(isEndgame && popcount(whitePawns) == 0 && popcount(blackPawns) == 0 && (materialScore * this->perspective) > 0) {
        //std::cout << "start mopup: " << board.sideToMove << std::endl;
        //calculate Manhattan-Distance (orthogonal distance between kings)
        Square whiteKingPos = bitboardToSquare(whiteKing);
        Square blackKingPos = bitboardToSquare(blackKing);

        int md = calculateOrthogonalDistance(whiteKingPos, blackKingPos);

        //std::cout << "before cmd: " << std::endl;
        int cmd = board.sideToMove == WHITE ? lookupCMD(blackKingPos) : lookupCMD(whiteKingPos); 
        //std::cout << "after cmd" << std::endl;

        mopUpEval = 20 * cmd + 8 * (14 - md);

        //std::cout << "return mopup" << std::endl;
        return ((materialScore * this->perspective) + mopUpEval);
    } 

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


int Evaluation::calculateOrthogonalDistance(Square s1, Square s2) {
    int file1 = s1 % 8; 
    int file2 = s2 % 8;

    int row1 = s1 / 8;
    int row2 = s2 / 8;

    int d = abs(row2 - row1) + abs(file2 - file1); 
    return d;
}

//CMD == Center-Manhattan-Distance
int Evaluation::lookupCMD(int square) {
    return arrCenterManhattanDistance[square];
}

int Evaluation::popcount(Bitboard bitboard) {
    return std::__libcpp_popcount(bitboard);
}
