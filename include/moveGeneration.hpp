#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include <iostream>
#include "../include/types.hpp"
#include "../include/board.hpp"
#include <vector>

class MoveGeneration {
    public:
        Board board;
        Bitboard ignoreOccupence;
        bool generatingKingMoves;

        //State
        //pins
        Pins pinnedPiecesWhite;
        Pins pinnedPiecesBlack;
        bool calculatedPinnedPiecesWhite;
        bool calculatedPinnedPiecesBlack;
        //attacks
        Bitboard attackedByWhite;
        Bitboard attackedByBlack;
        //check
        Check_Info check_info;

        u64 underAttackCounter;

        u64 nodes;

        MoveGeneration(Board board);
        MoveGeneration();

        std::vector<Move> generateMoves(const Board &board, Color color = BLACK, bool generateOnlyCaptures = false);
        u64 perft (int depth, Color color);

        void clearState();

        Bitboard generateRookMoves(Bitboard rooks = CURRENT_POSITION, Color color = BLACK);
        Bitboard generateBishopMoves(Bitboard bishops = CURRENT_POSITION, Color color = BLACK);
        Bitboard generateQueenMoves(Bitboard queens = CURRENT_POSITION, Color color = BLACK);

        Bitboard generatePawnMoves(Bitboard pawns = CURRENT_POSITION, Color color = BLACK);
        Bitboard generatePawnAttacks(Bitboard pawns = CURRENT_POSITION, Color color = BLACK);
        Bitboard generateEnPassentMoves(Bitboard pawns = CURRENT_POSITION, Color color = BLACK);

        Bitboard generateKnightAttacks(Bitboard knights = CURRENT_POSITION, Color color = BLACK);
        Bitboard generateKnightMoves(Bitboard knights = CURRENT_POSITION, Color color = BLACK);

        Bitboard generateKingMoves(Bitboard king = CURRENT_POSITION, Color color = BLACK);
        Bitboard generateKingAttacks(Bitboard king = CURRENT_POSITION, Color color = BLACK);

        Bitboard generateAttackedSquares(Color color = BLACK);
        Bitboard generateAttackedSquaresWithoutKing(Color color = BLACK);

        Check_Info isInCheck(Color color = BLACK);
        Attack_Info isUnderAttack(Square square, Color color = BLACK);
        Attack_Info isUnderAttack(Bitboard squareAsBitboard, Color color = BLACK); 

        Pins getPinnedPieces(Color color);
        Bitboard getAllLegalMovesOf(Bitboard pieces, Direction direction, Color color);
       
    private:

        Bitboard Northwest(Square square);
        Bitboard Northeast(Square square);
        Bitboard North(Square square);
        Bitboard Southwest(Square square);
        Bitboard Southeast(Square square);
        Bitboard South(Square square);
        Bitboard East(Square square);
        Bitboard West(Square square);

        Bitboard getLegalNorthwestMoves(Bitboard pieces, Color color);
        Bitboard getLegalNortheastMoves(Bitboard pieces, Color color);
        Bitboard getLegalSouthwestMoves(Bitboard pieces, Color color);
        Bitboard getLegalSoutheastMoves(Bitboard pieces, Color color);

        Bitboard getLegalNorthMoves(Bitboard pieces, Color color);
        Bitboard getLegalSouthMoves(Bitboard pieces, Color color);
        Bitboard getLegalWestMoves(Bitboard pieces, Color color);
        Bitboard getLegalEastMoves(Bitboard pieces, Color color);

        Bitboard checkForPinnedPieces(Bitboard kingMoves, Bitboard pieceMoves, Bitboard piece2Moves, PieceType pieceType, PieceType piece2Type, Direction direction, Color color, Pins &pins); 
        bool checkForEnPassenDiscoveredCheck(Bitboard targetPawn, Bitboard attackerPawn, Color color);

        u64 runPerft(int depth, int maxDepth, Color color);

};

#endif