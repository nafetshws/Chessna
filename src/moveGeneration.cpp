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

    attacks = (1ULL << (square + Direction::NORTH));
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

    attacks = (1ULL << (square + Direction::NORTH_EAST));
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

    attacks = (1ULL << (square + Direction::NORTH_WEST));
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

    attacks = (1ULL << (square - Direction::SOUTH));
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

    attacks = (1ULL << (square - Direction::SOUTH_EAST));
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

    attacks = (1ULL << (square - Direction::SOUTH_WEST));
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

    attacks = (1ULL << (square - Direction::WEST));
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

    attacks = (1ULL << (square + Direction::EAST));
    file++;

    while(file < 7) {
        attacks = attacks | (attacks << Direction::EAST);
        file++;
    }
    return attacks;
}


Bitboard MoveGeneration::generateBishopMoves(Bitboard bishops, Color color) {
    if(bishops == (0ULL-1)) bishops = (color == BLACK) ? this->board.blackBishops : this->board.whiteBishops;

    Bitboard allSinlgeMoves = EMPTY;
    Bitboard totalMoves = EMPTY;

    while(bishops > 0) {
        Bitboard finalNorthWestMoves = EMPTY;
        Bitboard finalNorthEastMoves = EMPTY;
        Bitboard finalSouthWestMoves  = EMPTY;
        Bitboard finalSouthEastMoves  = EMPTY;

        //clzll returns the number of leading zeros
        int index = 63 - __builtin_clzll(bishops);
        Bitboard northWestAttacks = MoveGeneration::Northwest(index);
        if(northWestAttacks != 0ULL) {
            Bitboard northWestBlockers = (northWestAttacks & this->board.getOccupied());
            if(northWestBlockers != 0ULL) {
                //ctzll returns the number of trailing zeros
                int northWestFirstBlockerIndex = __builtin_ctzll(northWestBlockers);
                Bitboard blockedMoves = MoveGeneration::Northwest(northWestFirstBlockerIndex);
                //if the first blocker is of the same color
                if((1ULL << northWestFirstBlockerIndex) == ((1ULL << northWestFirstBlockerIndex) & (color == BLACK ? this->board.getOccupiedByBlack() : this->board.getOccupiedByWhite()))) {
                    blockedMoves = (blockedMoves | (1ULL << northWestFirstBlockerIndex));
                }
                finalNorthWestMoves = (northWestAttacks ^ blockedMoves);
            } else {
                finalNorthWestMoves = northWestAttacks;
            }
        }

        Bitboard northEastAttacks = MoveGeneration::Northeast(index);
        if(northEastAttacks!= 0ULL) {
            Bitboard northEastBlockers = (northEastAttacks & this->board.getOccupied());
            if(northEastBlockers != 0ULL) {
                int northEastFirstBlockerIndex = __builtin_ctzll(northEastBlockers);
                Bitboard blockedMoves = MoveGeneration::Northeast(northEastFirstBlockerIndex);
                if((1ULL << northEastFirstBlockerIndex) == ((1ULL << northEastFirstBlockerIndex) & (color == BLACK ? this->board.getOccupiedByBlack() : this->board.getOccupiedByWhite()))) {
                    blockedMoves = (blockedMoves | (1ULL << northEastFirstBlockerIndex));
                }
                finalNorthEastMoves = (northEastAttacks ^ blockedMoves);
            } else {
                finalNorthEastMoves = northEastAttacks;
            }
        }

        Bitboard southWestAttacks = MoveGeneration::Southwest(index);
        if(southWestAttacks != 0ULL) {
            Bitboard southWestBlockers = (southWestAttacks & this->board.getOccupied());
            if(southWestBlockers != 0ULL) {
                int southWestFirstBlockerIndex = 63 -__builtin_clzll(southWestBlockers);
                Bitboard blockedMoves = MoveGeneration::Southwest(southWestFirstBlockerIndex);
                if((1ULL << southWestFirstBlockerIndex) == ((1ULL << southWestFirstBlockerIndex) & (color == BLACK ? this->board.getOccupiedByBlack() : this->board.getOccupiedByWhite()))) {
                    blockedMoves = (blockedMoves | (1ULL << southWestFirstBlockerIndex));
                }
                finalSouthWestMoves = (southWestAttacks ^ blockedMoves);
            } else {
                finalSouthWestMoves = southWestAttacks;
            }
        }

        Bitboard southEastAttacks = MoveGeneration::Southeast(index);
        if(southEastAttacks!= 0ULL) {
            Bitboard southEastBlockers = (southEastAttacks & this->board.getOccupied());
            if(southEastBlockers != 0ULL) {
                int southEastFirstBlockerIndex = 63 - __builtin_clzll(southEastBlockers);
                Bitboard blockedMoves = MoveGeneration::Southeast(southEastFirstBlockerIndex);
                if((1ULL << southEastFirstBlockerIndex) == ((1ULL << southEastFirstBlockerIndex) & (color == BLACK ? this->board.getOccupiedByBlack() : this->board.getOccupiedByWhite()))) {
                    blockedMoves = (blockedMoves | (1ULL << southEastFirstBlockerIndex));
                }
                finalSouthEastMoves = (southEastAttacks ^ blockedMoves);
            } else {
                finalSouthEastMoves = southEastAttacks;
            }
        }

        std::cout << "north west moves: " << finalNorthWestMoves << std::endl;
        std::cout << "north east moves: " << finalNorthEastMoves << std::endl;
        std::cout << "south west moves: " << finalSouthWestMoves << std::endl;
        std::cout << "south east moves: " << finalSouthEastMoves << std::endl;

        allSinlgeMoves = finalNorthWestMoves | finalNorthEastMoves | finalSouthWestMoves | finalSouthEastMoves;
        totalMoves = (totalMoves | allSinlgeMoves);
        //erase bishop
        bishops = ((1ULL << index) ^ bishops);
    }
    return totalMoves;
}

Bitboard MoveGeneration::generateRookMoves(Bitboard rooks, Color color) {
    if(rooks == (0ULL-1)) rooks = this->board.blackRooks;

    Bitboard allSinlgeMoves = EMPTY;
    Bitboard totalMoves = EMPTY;

    while(rooks > 0) {
        Bitboard finalNorthMoves = EMPTY;
        Bitboard finalSouthMoves = EMPTY;
        Bitboard finalWestMoves  = EMPTY;
        Bitboard finalEastMoves  = EMPTY;

        //clzll returns the number of leading zeros
        int index = 63 - __builtin_clzll(rooks);
        //Bitboard moves = MoveGeneration::North(index) | MoveGeneration::South(index) | MoveGeneration::East(index) | MoveGeneration::West(index);
        Bitboard northAttacks = MoveGeneration::North(index);
        if(northAttacks != 0ULL) {
            Bitboard northBlockers = (northAttacks & this->board.getOccupied());
            if(northBlockers != 0ULL) {
                //ctzll returns the number of trailing zeros
                int northFirstBlockerIndex = __builtin_ctzll(northBlockers);
                Bitboard blockedMoves = MoveGeneration::North(northFirstBlockerIndex);
                //if the first blocker is of the same color
                if((1ULL << northFirstBlockerIndex) == ((1ULL << northFirstBlockerIndex) & (color == BLACK ? this->board.getOccupiedByBlack() : this->board.getOccupiedByWhite()))) {
                    blockedMoves = (blockedMoves | (1ULL << northFirstBlockerIndex));
                }
                finalNorthMoves = (northAttacks ^ blockedMoves);
            } else {
                finalNorthMoves = northAttacks;
            }
        }

        Bitboard southAttacks = MoveGeneration::South(index);
        if(southAttacks!= 0ULL) {
            Bitboard southBlockers = (southAttacks & this->board.getOccupied());
            if(southBlockers != 0ULL) {
                //ctzll returns the number of leading zeros
                int southFirstBlockerIndex = 63 - __builtin_clzll(southBlockers);
                Bitboard blockedMoves = MoveGeneration::South(southFirstBlockerIndex);
                //if the first blocker is of the same color
                if((1ULL << southFirstBlockerIndex) == ((1ULL << southFirstBlockerIndex) & (color == BLACK ? this->board.getOccupiedByBlack() : this->board.getOccupiedByWhite()))) {
                    blockedMoves = (blockedMoves | (1ULL << southFirstBlockerIndex));
                }
                finalSouthMoves = (southAttacks ^ blockedMoves);
            } else {
                finalSouthMoves = southAttacks;
            }
        }

        Bitboard westAttacks = MoveGeneration::West(index);
        if(westAttacks != 0ULL) {
            Bitboard westBlockers = (westAttacks & this->board.getOccupied());
            if(westBlockers != 0ULL) {
                //ctzll returns the number of trailing zeros
                int westFirstBlockerIndex = 63 -__builtin_clzll(westBlockers);
                Bitboard blockedMoves = MoveGeneration::West(westFirstBlockerIndex);
                //if the first blocker is of the same color
                if((1ULL << westFirstBlockerIndex) == ((1ULL << westFirstBlockerIndex) & (color == BLACK ? this->board.getOccupiedByBlack() : this->board.getOccupiedByWhite()))) {
                    blockedMoves = (blockedMoves | (1ULL << westFirstBlockerIndex));
                }
                finalWestMoves = (westAttacks ^ blockedMoves);
            } else {
                finalWestMoves = westAttacks;
            }
        }

        Bitboard eastAttacks = MoveGeneration::East(index);
        if(eastAttacks!= 0ULL) {
            Bitboard eastBlockers = (eastAttacks & this->board.getOccupied());
            if(eastBlockers != 0ULL) {
                //ctzll returns the number of leading zeros
                int eastFirstBlockerIndex = __builtin_ctzll(eastBlockers);
                Bitboard blockedMoves = MoveGeneration::East(eastFirstBlockerIndex);
                //if the first blocker is of the same color
                if((1ULL << eastFirstBlockerIndex) == ((1ULL << eastFirstBlockerIndex) & (color == BLACK ? this->board.getOccupiedByBlack() : this->board.getOccupiedByWhite()))) {
                    blockedMoves = (blockedMoves | (1ULL << eastFirstBlockerIndex));
                }
                finalEastMoves = (eastAttacks ^ blockedMoves);
            } else {
                finalEastMoves = eastAttacks;
            }
        }
        allSinlgeMoves = finalNorthMoves | finalSouthMoves | finalWestMoves | finalEastMoves;
        totalMoves = (totalMoves | allSinlgeMoves);
        //erase rook
        rooks = ((1ULL << index) ^ rooks);
    }
    return totalMoves;
}

Bitboard MoveGeneration::generateQueenMoves(Bitboard queens, Color color) {
    if(queens == (0ULL-1)) queens = (color == BLACK) ? this->board.blackQueen : this->board.whiteQueen;
    //std::cout << "rook moves: " << generateRookMoves(queens, color) << std::endl;
    //std::cout << "bishop moves: " << generateBishopMoves(queens, color) << std::endl;
    return (generateBishopMoves(queens, color) | generateRookMoves(queens, color));
}

Bitboard MoveGeneration::generatePawnMoves(Bitboard pawns, Color color) {
    if(color == WHITE) {
        //pawn pushes
        pawns = (pawns == 0ULL-1) ? this->board.whitePawns : pawns;
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
    } else {
        //pawn pushes
        pawns = (pawns == 0ULL-1) ? this->board.blackPawns : pawns;
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
}

Bitboard MoveGeneration::generateKnightMoves(Bitboard knights, Color color) {

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

    Bitboard moves;
    if(color == BLACK) {
        moves = (attacks & ~this->board.getOccupiedByBlack());
    } else {
        moves = (attacks & ~this->board.getOccupiedByWhite());
    }

    return moves;
}

Bitboard MoveGeneration::generateKingMoves(Bitboard king, Color color) {
    //north
    Bitboard northAttack = ((king & ~(RANK_8)) << NORTH);
    //west
    Bitboard westAttack = ((king & ~(FILE_A)) << WEST);
    //south
    Bitboard southAttack = ((king & ~(RANK_1)) >> SOUTH);
    //east
    Bitboard eastAttack = ((king & ~(FILE_H)) >> EAST);

    Bitboard castle = EMPTY;

    //castling
    if(color == WHITE) {
        if(this->board.castlingAbillity.find('K') != std::string::npos) {
            if((~this->board.getOccupied() & WHITE_CASTLE_KING_MASK) == WHITE_CASTLE_KING_MASK) {
                //castleKingSide(color);
                castle = castle | (this->board.whiteKing << 2);
            }
        } 
        if(this->board.castlingAbillity.find('Q') != std::string::npos) {
            if((~this->board.getOccupied() & WHITE_CASTLE_QUEEN_MASK) == WHITE_CASTLE_QUEEN_MASK) {
                //castleQueenSide(color);
                castle = castle | (this->board.whiteKing >> 2);
            }
        } 
    } else {
        if(this->board.castlingAbillity.find('k') != std::string::npos) {
            if((~this->board.getOccupied() & BLACK_CASTLE_KING_MASK) == BLACK_CASTLE_KING_MASK) {
                //castleKingSide(color);
                castle = castle | (this->board.whiteKing << 2);
            }
        } 
        if(this->board.castlingAbillity.find('q') != std::string::npos) {
            if((~this->board.getOccupied() & BLACK_CASTLE_QUEEN_MASK) == BLACK_CASTLE_QUEEN_MASK) {
                //castleQueenSide(color);
                castle = castle | (this->board.whiteKing >> 2);
            }
        } 
    }

    Bitboard attacks = northAttack | westAttack | southAttack | eastAttack | castle;

    Bitboard moves;
    if(color == BLACK) {
        moves = (attacks & ~this->board.getOccupiedByBlack());
    } else {
        moves = (attacks & ~this->board.getOccupiedByWhite());
    }

    return moves;
}


void MoveGeneration::castleKingSide(Color color) {
    if(color == WHITE) {
        //move king
        this->board.whiteKing = (this->board.whiteKing << 2);
        //move rook
        this->board.whiteRooks = (((~(1ULL << 7)) & this->board.whiteRooks) | (1ULL << 5));
    } else {
        //move king
        this->board.blackKing = (this->board.blackKing << 2);
        //move rook
        this->board.whiteRooks = (((~(1ULL << 63)) & this->board.whiteRooks) | (1ULL << 61));
    }
}

void MoveGeneration::castleQueenSide(Color color) {
    if(color == WHITE) {
        //move king
        this->board.whiteKing = (this->board.whiteKing >> 2);
        //move rook
        this->board.whiteRooks = (((~(1ULL)) & this->board.whiteRooks) | (1ULL << 3));
    } else {
        //move king
        this->board.blackKing = (this->board.blackKing >> 2);
        //move rook
        this->board.whiteRooks = (((~(1ULL << 56)) & this->board.whiteRooks) | (1ULL << 59));
    }
}