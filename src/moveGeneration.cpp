#include <vector>
#include "moveGeneration.hpp"
#include "board.hpp"
#include "move.hpp"
#include "types.hpp"

MoveGeneration::MoveGeneration(Board board) {
    this->board = board;
}

Bitboard MoveGeneration::North(Square square) {
    int rank = square / 8;
    Bitboard attacks = EMPTY;

    if(rank == 7) return attacks;

    attacks = (1 << (square + Direction::NORTH));
    rank++;

    while(rank < 7) {
        attacks = attacks | (attacks << Direction::NORTH); 
        rank++;
    }

    return attacks;
}

Bitboard MoveGeneration::Northeast(Square square) {
    int rank = square / 8;
    int file = square % 8;
    Bitboard attacks = EMPTY;

    if(rank == 7 || file == 7) return attacks;

    attacks = (1 << (square + Direction::NORTH_EAST));
    rank++;
    file++;

    while(rank < 7 && file < 7) {
        attacks = attacks | (attacks << Direction::NORTH_EAST); 
        rank++;
        file++;
    }

    return attacks;
}

Bitboard MoveGeneration::Northwest(Square square) {
    int rank = square / 8;
    int file = square % 8;
    Bitboard attacks = EMPTY;

    if(rank == 7 || file == 0) return attacks;

    attacks = (1UL << (square + Direction::NORTH_WEST));
    rank++;
    file--;

    while(rank < 7 && file > 0) {
        attacks = attacks | (attacks << Direction::NORTH_WEST); 
        rank++;
        file--;
    }

    return attacks;
}


Bitboard MoveGeneration::South(Square square) {
    int rank = square / 8;
    Bitboard attacks = EMPTY;

    if(rank == 0) return attacks;

    attacks = (1UL << (square - Direction::SOUTH));
    rank--;

    while(rank > 0) {
        attacks = attacks | (attacks >> Direction::SOUTH); 
        rank--;
    }

    return attacks;
}

Bitboard MoveGeneration::Southeast(Square square) {
    int rank = square / 8;
    int file = square % 8;
    Bitboard attacks = EMPTY;

    if(rank == 0 || file == 7) return attacks;

    attacks = (1UL << (square - Direction::SOUTH_EAST));
    rank--;
    file++;

    while(rank > 0 && file < 7) {
        attacks = attacks | (attacks >> Direction::SOUTH_EAST); 
        rank--;
        file++;
    }

    return attacks;
}

Bitboard MoveGeneration::Southwest(Square square) {
    int rank = square / 8;
    int file = square % 8;
    Bitboard attacks = EMPTY;

    if(rank == 0 || file == 0) return attacks;

    attacks = (1UL << (square - Direction::SOUTH_WEST));
    rank--;
    file--;

    while(rank > 0 && file > 0) {
        attacks = attacks | (attacks >> Direction::SOUTH_WEST); 
        rank--;
        file--;
    }

    return attacks;
}

Bitboard MoveGeneration::West(Square square) {
    
    int file = square % 8;
    Bitboard attacks = EMPTY; 

    if(file == 0) return attacks;

    attacks = (1UL << (square - Direction::WEST));
    file--;

    while(file > 0) {
        attacks = attacks | (attacks >> Direction::WEST);
        file--;
    }

    return attacks;

}

Bitboard MoveGeneration::East(Square square) {
    int file = square % 8;
    Bitboard attacks = EMPTY; 

    if(file == 7) return attacks;

    attacks = (1UL << (square + Direction::EAST));
    file++;

    while(file < 7) {
        attacks = attacks | (attacks << Direction::EAST);
        file++;
    }

    return attacks;

}

void MoveGeneration::generateRookMoves(Bitboard rooks) {
    //if default value -> use rooks on board
    if(rooks == (0UL-1)) rooks = this->board.blackRooks;

}

Bitboard MoveGeneration::generatePawnMovesBlack() {
    //pawn pushes
    Bitboard pawns = this->board.blackPawns;
    //single push
    Bitboard pawnSinglePushes = (pawns >> SOUTH) & (~board.getOccupied());
    //double push
    Bitboard pawnDoublePushes = ((pawns & RANK_7) >> 2*SOUTH) & (~board.getOccupied());
    Bitboard pushes = (pawnSinglePushes | pawnDoublePushes);

    //SouthWest
    Bitboard swAttacks = ((pawns & ~FILE_A) >> SOUTH_WEST) & board.getOccupiedByWhite();
    //SouthEast
    Bitboard seAttacks = ((pawns & ~ FILE_H) >> SOUTH_EAST) & board.getOccupiedByWhite();
    Bitboard attacks = swAttacks | seAttacks;

    return (pushes | attacks);
}

Bitboard MoveGeneration::generatePawnMovesWhite() {
    //pawn pushes
    Bitboard pawns = this->board.whitePawns;
    //single push
    Bitboard pawnSinglePushes = (pawns << NORTH) & (~board.getOccupied());
    //double push
    Bitboard pawnDoublePushes = ((pawns & RANK_2) >> 2*NORTH) & (~board.getOccupied());
    Bitboard pushes = (pawnSinglePushes | pawnDoublePushes);

    //SouthWest
    Bitboard swAttacks = ((pawns & ~FILE_A) << NORTH_WEST) & board.getOccupiedByBlack();
    //SouthEast
    Bitboard seAttacks = ((pawns & ~ FILE_H) << NORTH_EAST) & board.getOccupiedByBlack();
    Bitboard attacks = swAttacks | seAttacks;

    return (pushes | attacks);
}

Bitboard MoveGeneration::generateKnightMoves(Bitboard knights) {

    //noNoEa
    Bitboard noNoEaAttacks = ((knights & ~(RANK_7 | RANK_8 | FILE_H)) << NORTH_NORTH_EAST);
    //noEaEa
    Bitboard noEaEaAttacks = ((knights & ~(RANK_8 | FILE_G | FILE_H)) << NORTH_EAST_EAST);
    //soEaEa
    Bitboard soEaEaAttacks = ((knights & ~(RANK_1 | FILE_G | FILE_H)) >> SOUTH_EAST_EAST);
    //soSoEa
    Bitboard soSoEaAttacks = ((knights & ~(RANK_1 | RANK_2 | FILE_H)) >> SOUTH_SOUTH_EAST);
    //soSoWe
    Bitboard soSoWeAttacks = ((knights & ~(RANK_1 | RANK_2 | FILE_A)) >> SOUTH_SOUTH_WEST);
    //soWeWe
    Bitboard SoWeWeAttacks = ((knights & ~(RANK_1 | FILE_A | FILE_B)) >> SOUTH_WEST_WEST);
    //noWeWe
    Bitboard noWeWeAttacks = ((knights & ~(RANK_8 | FILE_A | FILE_B)) << NORTH_WEST_WEST);
    //noNoWe
    Bitboard noNoWeAttacks = ((knights & ~(RANK_7 | RANK_8 | FILE_A)) << NORTH_NORTH_WEST);

    Bitboard attacks = noNoEaAttacks | noEaEaAttacks | soEaEaAttacks | soSoEaAttacks | soSoWeAttacks | SoWeWeAttacks | noWeWeAttacks | noNoWeAttacks;
    Bitboard moves = (attacks & ~this->board.getOccupiedByBlack());
    return moves;
}

Bitboard MoveGeneration::generateKingMoves(Bitboard king) {
    //north
    Bitboard northAttack = ((king & ~(RANK_8)) << NORTH);
    //west
    Bitboard westAttack = ((king & ~(FILE_A)) << WEST);
    //south
    Bitboard southAttack = ((king & ~(RANK_1)) >> SOUTH);
    //east
    Bitboard eastAttack = ((king & ~(FILE_H)) >> EAST);

    Bitboard attacks = northAttack | westAttack | southAttack | eastAttack;
    Bitboard moves = (attacks & ~this->board.getOccupiedByBlack());
    return moves;
}