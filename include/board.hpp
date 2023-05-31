#ifndef BOARD_H
#define BOARD_H

#include "../include/types.hpp"
#include <string>

struct Board {
    Bitboard whitePawns, whiteRooks, whiteKnights, whiteBishops, whiteQueen, whiteKing;
    Bitboard blackPawns, blackRooks, blackKnights, blackBishops, blackQueen, blackKing;

    std::string sideToMove;
    std::string castlingAbillity;
    int enPassentTargetSquare;
    int halfMoveClock;
    int fullMoveCounter;


    Board(std::string fen);
    Board();
    void initBoard(std::string fen = DEFAULT_FEN);
    Bitboard getOccupied();
    Bitboard getOccupiedByWhite();
    Bitboard getOccupiedByBlack();

    void castleKingSide(Color color = BLACK);
    void castleQueenSide(Color color = BLACK);

    void printBoard(Bitboard board); 
    void prettyPrintBoard();

};

#endif