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

        Bitboard generateRookMoves(Bitboard rooks = 0ULL-1);
        Bitboard generateBishopMoves(Bitboard bishops = 0ULL-1);
        Bitboard generateQueenMoves(Bitboard queens = 0ULL-1);
        std::vector<Move> generatBishopfMoves(Bitboard bishops);

        Bitboard generatePawnMovesBlack();
        Bitboard generatePawnMovesWhite();

        Bitboard generateKnightMoves(Bitboard knights);

        Bitboard generateKingMoves(Bitboard king);
       
    private:
        Board board;
};

#endif