#include <random>
#include <iostream>
#include <climits>

#include "../include/zobrist.hpp"
#include "../include/board.hpp"
#include "../include/types.hpp"

std::mt19937_64 Zobrist::gen = std::mt19937_64(SEED);
u64 Zobrist::pieces[64][6][2];
u64 Zobrist::castlingRights[4];
u64 Zobrist::enPassentFile[8];
u64 Zobrist::sideToMove; 

void Zobrist::init() {
    //generate pieces array
    for(int square = 0; square < 64; square++) {
        for(int piece = 0; piece < 6; piece++) {
            for(int color = 0; color < 2; color++) {
                pieces[square][piece][color] = generateRandom64BitNumber();
            }
        }
    }

    //generate castling rights
    for(int i = 0; i < 4; i++) {
        castlingRights[i] = generateRandom64BitNumber();
    }

    //generate en passent file
    for(int file = 0; file < 8; file++) {
        enPassentFile[file] = generateRandom64BitNumber();
    }

    //generate sideToMove - use only if it is blacks turn
    sideToMove = generateRandom64BitNumber();
}

u64 Zobrist::createZobristKey(Board board) {
    u64 zobristKey = 0;

    //xor pieces
    for(int i = 0; i < 64; i++) {
        Piece piece = board.findPiece(i);
        if(piece.type == UNOCCUPIED) continue;
        u64 rand = pieces[i][piece.type][piece.color];
        zobristKey ^= rand;
    }

    //castling rights
    if(board.getKingSideCastleAbility(WHITE)) zobristKey ^= castlingRights[0];
    if(board.getQueenSideCastleAbility(WHITE)) zobristKey ^= castlingRights[1];
    if(board.getKingSideCastleAbility(BLACK)) zobristKey ^= castlingRights[2];
    if(board.getQueenSideCastleAbility(BLACK)) zobristKey ^= castlingRights[3];

    //en passent file
    if(board.enPassentTargetSquare != -1) {
        zobristKey ^= enPassentFile[board.enPassentTargetSquare % 8];
    }

    //side to move
    if(board.sideToMove == BLACK) zobristKey ^= sideToMove;

    return zobristKey;
}

u64 Zobrist::generateRandom64BitNumber() {
    std::uniform_int_distribution<unsigned long long int> dist(1, ULLONG_MAX);
    return dist(gen); 
}
