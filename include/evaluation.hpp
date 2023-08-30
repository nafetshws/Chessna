#ifndef EVALUATION_H
#define EVALUATION_H

#include "./board.hpp"

const int KING_VALUE = 20000;
const int QUEEN_VALUE = 900;
const int ROOK_VALUE = 500;
const int BISHOP_VAUE = 330; 
const int KNIGHT_VALUE = 320;
const int PAWN_VALUE = 100;

class Evaluation {
    public:
        int sideToMove;

        Evaluation();

        int evaluatePosition(Board board);
        int popcount(Bitboard bitboard);

};

#endif