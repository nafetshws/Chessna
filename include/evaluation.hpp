#ifndef EVALUATION_H
#define EVALUATION_H

#include "./board.hpp"

class Evaluation {
    public:
        int sideToMove;

        Evaluation();

        int evaluatePosition(Board board);

        int popcount(Bitboard bitboard);
};

const int KING_VALUE = 100000;
const int QUEEN_VALUE = 900;
const int ROOK_VALUE = 500;
const int BISHOP_VAUE = 300; 
const int KNIGHT_VALUE = 300;
const int PAWN_VALUE = 100;

#endif