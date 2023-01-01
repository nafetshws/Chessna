#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include <iostream>
#include "types.hpp"
#include "board.hpp"
#include "move.hpp"
#include <vector>

class MoveGeneration {
    public:
        Board board;

        MoveGeneration(Board board);

        Bitboard generateRookMoves(Bitboard rooks = 0ULL-1, Color color = BLACK);
        Bitboard generateBishopMoves(Bitboard bishops = 0ULL-1, Color color = BLACK);
        Bitboard generateQueenMoves(Bitboard queens = 0ULL-1, Color color = BLACK);

        Bitboard generatePawnMoves(Bitboard pawns = 0ULL-1, Color color = BLACK);
        Bitboard generatePawnAttacks(Bitboard pawns = 0ULL-1, Color color = BLACK);
        Bitboard generateKnightMoves(Bitboard knights = 0ULL-1, Color color = BLACK);
        Bitboard generateKingMoves(Bitboard king = 0ULL-1, Color color = BLACK);

        Bitboard generateAttackedSquares(Color color = BLACK);
       
    private:

        Bitboard Northwest(Square square);
        Bitboard Northeast(Square square);
        Bitboard North(Square square);

        Bitboard Southwest(Square square);
        Bitboard Southeast(Square square);
        Bitboard South(Square square);

        Bitboard East(Square square);
        Bitboard West(Square square);

};

#endif