#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include <iostream>
#include "types.hpp"
#include "board.hpp"
#include "move.hpp"
#include <vector>

class MoveGeneration {
    public:
        MoveGeneration(Board board);
        Bitboard Northwest(Square square);
        Bitboard Northeast(Square square);
        Bitboard North(Square square);

        Bitboard Southwest(Square square);
        Bitboard Southeast(Square square);
        Bitboard South(Square square);

        Bitboard East(Square square);
        Bitboard West(Square square);

        void generateRookMoves(Bitboard rooks = 0UL-1);
        std::vector<Move> generatBishopfMoves(Bitboard bishops);

        Bitboard generatePawnPushes();
        Bitboard generatePawnAttacks();
        Bitboard generatePawnMoves();

        Bitboard generateKnightMoves();

       
    private:
        Board board;
};

#endif