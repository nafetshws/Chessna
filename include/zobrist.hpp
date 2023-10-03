#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <random>
#include "../include/types.hpp"
#include "../include/board.hpp"

const u64 SEED = 438456954835;

class Zobrist {
    public:
        static std::mt19937_64 gen;

        static u64 pieces[64][6][2];
        static u64 castlingRights[4];
        static u64 enPassentFile[8];
        static u64 sideToMove;

        static void init();

        static u64 createZobristKey(Board board);
        static u64 generateRandom64BitNumber();

    private:

};

#endif