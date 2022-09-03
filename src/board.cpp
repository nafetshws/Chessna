#include <iostream>
#include "types.hpp"
#include "board.hpp"

struct Board {
    Bitboard whitePawns, whiteRooks, whiteKnights, whiteBishops, whiteQueen, whiteKing;
    Bitboard blackPawns, blackRooks, blackKnights, blackBishops, blackQueen, blackKing;

    void initBoard() {
        //default layout
        whitePawns = 0xff00;
        whiteRooks = 0x81;
        whiteKnights = 0x42;
        whiteBishops = 0x24;
        whiteQueen = 0x8;
        whiteKing = 0x10;

        blackPawns = 0xff000000000000;
        blackRooks = 0x8100000000000000;
        blackKnights = 0x4200000000000000;
        blackBishops = 0x2400000000000000;
        blackQueen = 0x800000000000000;
        blackKing = 0x1000000000000000;
    }

    void initBoard(std::string fen) {
        //implement later
        initBoard();
    }
};
