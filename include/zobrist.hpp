#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <random>
#include "../include/types.hpp"
#include "../include/board.hpp"


const u64 SEED = 438456954835;

class Zobrist {
    public:
        Board board;
        std::mt19937_64 gen;

        Zobrist(Board board);
        u64 createZobristKey();
        u64 generateRandom64BitNumber();

    private:
        u64 pieces[64][6];
        u64 castlingRights[4];
        u64 enPassentFile[8];
        u64 sideToMove;

};

#endif