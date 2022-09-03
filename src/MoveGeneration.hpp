#include <iostream>
#include "types.hpp"
#include "board.hpp"

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
    private:
        Board board;
};