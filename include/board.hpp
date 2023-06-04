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
    Bitboard getOccupiedBy(Color color = BLACK);
    Bitboard getOccupiedWithoutWhiteKing();
    Bitboard getOccupiedWithoutBlackKing();

    bool getKingSideCastleAbility(Color color = BLACK);
    bool getQueenSideCastleAbility(Color color = BLACK);
    void castleKingSide(Color color = BLACK);
    void castleQueenSide(Color color = BLACK);

    void printBoard(Bitboard board); 
    void prettyPrintBoard();

    Bitboard getPawns(Color color);
    Bitboard getRooks(Color color);
    Bitboard getKnights(Color color);
    Bitboard getBishops(Color color);
    Bitboard getQueens(Color color);
    Bitboard getKing(Color color);
};



#endif