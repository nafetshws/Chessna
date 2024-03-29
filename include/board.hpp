#ifndef BOARD_H
#define BOARD_H

#include "../include/types.hpp"
#include <string>
#include <map>

struct Board {
    Bitboard whitePawns, whiteRooks, whiteKnights, whiteBishops, whiteQueen, whiteKing;
    Bitboard blackPawns, blackRooks, blackKnights, blackBishops, blackQueen, blackKing;

    Color sideToMove;
    std::string castlingAbillity;
    Square enPassentTargetSquare;
    int halfMoveClock;
    int fullMoveCounter;

    u64 zobristKey;

    Board(const std::string &fen);
    Board();

    void initBoard(const std::string &fen); 
    Piece findPiece(Square s);

    void makeMove(Move move);

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
    void removeKingSideCastleAbillity(Color color = BLACK);
    void removeQueenSideCastleAbillity(Color color = BLACK);

    void makeQueenPromotion(Move move);
    void makeRookPromotion(Move move);
    void makeBishopPromotion(Move move);
    void makeKnightPromotion(Move move);

    void printBoard(Bitboard board); 
    void prettyPrintBoard();

    Bitboard getPawns(Color color) const;
    Bitboard getRooks(Color color) const;
    Bitboard getKnights(Color color) const;
    Bitboard getBishops(Color color) const;
    Bitboard getQueens(Color color) const;
    Bitboard getKing(Color color) const;

    PieceType getPieceTypeOfSquare(Square square, Color color);
    PieceType getPieceTypeOfSquare(Bitboard b, Color color); 

    void updateZobristKey(Move move, Square prevEnPassentSquare);
    void updateZobristPieces(Move move);
    void updateZobristCastling(Move move);
    void updateZobristEnPassent(Square prevEnPassentSquare); 
    void updateZobristSideToMove();
    void updateZobristPromotion(Move move);
};



#endif
