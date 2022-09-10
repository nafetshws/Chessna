#ifndef BOARD_H
#define BOARD_H

#include "types.hpp"
#include <string>

struct Board {
    Bitboard whitePawns, whiteRooks, whiteKnights, whiteBishops, whiteQueen, whiteKing;
    Bitboard blackPawns, blackRooks, blackKnights, blackBishops, blackQueen, blackKing;

    std::string sideToMove;
    std::string castlingAbillity;
    std::string enPassentTargetSquare;
    int halfMoveClock;
    int fullMoveCounter;

    void initBoard();
    void initBoard(std::string fen);
    Bitboard getOccupied();
    Bitboard getOccupiedByWhite();
    Bitboard getOccupiedByBlack();

    void printBoard(Bitboard board); 

};

#endif