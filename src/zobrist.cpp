#include <random>
#include <iostream>
#include <climits>

#include "../include/zobrist.hpp"
#include "../include/types.hpp"
#include "../include/board.hpp"

Zobrist::Zobrist(Board board) {
    this->board = board;

    this->gen = std::mt19937_64(SEED);
    //initialize array with random numbers

    //generate pieces array
    for(int square = 0; square < 64; square++) {
        for(int piece = 0; piece < 6; piece++) {
            this->pieces[square][piece] = generateRandom64BitNumber();
        }
    }

    //generate castling rights
    for(int i = 0; i < 16; i++) {
        this->castlingRights[i] = generateRandom64BitNumber();
    }

    //generate en passent file
    for(int file = 0; file < 8; file++) {
        this->enPassentFile[file] = generateRandom64BitNumber();
    }

    //generate sideToMove - use only if it is blacks turn
    this->sideToMove = generateRandom64BitNumber();

}

u64 Zobrist::createZobristKey() {
    u64 zobristKey = 0;

    //xor pieces
    for(int i = 0; i < 64; i++) {
        Piece piece = board.findPiece(i);
        if(piece.type == UNOCCUPIED) continue;
        u64 rand = this->pieces[i][piece.type];
        zobristKey ^= rand;
    }

    //castling rights
    if(this->board.getKingSideCastleAbility(WHITE)) zobristKey ^= this->castlingRights[0];
    if(this->board.getQueenSideCastleAbility(WHITE)) zobristKey ^= this->castlingRights[1];
    if(this->board.getKingSideCastleAbility(BLACK)) zobristKey ^= this->castlingRights[2];
    if(this->board.getQueenSideCastleAbility(BLACK)) zobristKey ^= this->castlingRights[3];

    //en passent file
    zobristKey ^= this->enPassentFile[this->board.enPassentTargetSquare % 8];

    //side to move
    if(board.sideToMove == BLACK) zobristKey ^= this->sideToMove;

    return zobristKey;
}

u64 Zobrist::generateRandom64BitNumber() {
    std::uniform_int_distribution<unsigned long long int> dist(1, ULLONG_MAX);
    return dist(this->gen); 
}





