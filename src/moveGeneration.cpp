#include <vector>
#include "../include/moveGeneration.hpp"
#include "../include/board.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

MoveGeneration::MoveGeneration(Board board) {
    this->board = board;
}

std::vector<Move> MoveGeneration::generateMoves(Color color) {
    std::vector<Move> moves;
    //TODO
    //en passent check evasion
    //move types are not accurate!!

    //generate all king moves
    Bitboard kingMoves = generateKingMoves(CURRENT_POSITION, color);
    Square origin = __builtin_ctzll(this->board.getKing(color));

    //transform moves from bitboard into a vector
    while(kingMoves != 0) {
        Square destination = __builtin_ctzll(kingMoves);
        moves.push_back(Move(origin, destination, PieceType::KING, color, MoveType::QUIET));
        //delete move from bitboard
        kingMoves = ~(1ULL << destination) & kingMoves;
    }

    /*
    * Generate moves when <<color>> is in check
    */
    Check_Info check_info = isInCheck(color);

    //if the king is in double check he has to move
    if(check_info.numberOfChecks >= 2) return moves;

    else if(check_info.numberOfChecks == 1) {
        //Possiblites to react when in check:
        //1. move king out of check -> already in moves
        //2. capture the checking piece
        //3. block the checking piece (only rooks, bishops and queens)

        //2
        Square checkingPiecePos = check_info.moves.at(0).origin;
        //find every move that captures the checking piece
        Attack_Info a = isUnderAttack((Square)checkingPiecePos, getOppositeColor(color));
        //add those moves to the vector
        moves.insert(moves.end(), a.moves.begin(), a.moves.end());

        //3
        PieceType checkingPieceType = check_info.moves.at(0).pieceType;
        //only rooks, bishops and queens can be blocked
        if(checkingPieceType == QUEEN || checkingPieceType == ROOK || checkingPieceType == BISHOP) { 
            Bitboard kingAsSlidingPieceMoves; 
            //there could be multiple queens on the board -> look up pos of checking piece
            Bitboard enemyMoves;
            switch(checkingPieceType) {
                case QUEEN:
                    enemyMoves = generateQueenMoves(1ULL << check_info.moves.at(0).origin, getOppositeColor(color));
                    kingAsSlidingPieceMoves = generateQueenMoves(this->board.getKing(color), color);
                    break;
                case ROOK:
                    enemyMoves = generateRookMoves(1ULL << check_info.moves.at(0).origin, getOppositeColor(color));
                    kingAsSlidingPieceMoves = generateRookMoves(this->board.getKing(color), color);
                    break;
                case BISHOP:
                    enemyMoves = generateBishopMoves(1ULL << check_info.moves.at(0).origin, getOppositeColor(color));
                    kingAsSlidingPieceMoves = generateBishopMoves(this->board.getKing(color), color);
                    break;
                default:
                    std::cout << "Error while calculating blocking pieces" << std::endl;
                    break;
            }

            //the intersection of the enemy pieces moves and the king as the sliding piece move mark the squares that can block the enemy piece
            Bitboard intersectionRay = kingAsSlidingPieceMoves & enemyMoves;

            //transform bitboard of squares to block the attacker into moves by the attacked player
            while(intersectionRay != 0) {
                Square destination = __builtin_ctzll(intersectionRay);
                Attack_Info a_info = isUnderAttack(destination, getOppositeColor(color));
                moves.insert(moves.end(), a_info.moves.begin(), a_info.moves.end());
                //for(int i = 0; i < a_info.numberOfAttacks; i++) {
                //    //prevent diagonal pawn moves without enemy piece to capture
                //    if(a_info.moves.at(i).pieceType == PAWN && (squareToBitboard(destination) & this->board.getOccupiedBy(getOppositeColor(color))) == 0) continue;
                //    moves.push_back(Move(a_info.pieces.at(i).pos, destination));
                //}
                //remove from intersectionRay
                intersectionRay = ~(1ULL << destination) & intersectionRay;
            }
        }

        return moves;
    }

    /*
    * Generate mvoes if the king isn't in check
    */

    //generate knight moves
    Bitboard knights = this->board.getKnights(color);
    while(knights != 0) {
        Square origin = __builtin_ctzll(knights);
        Bitboard knight = 1ULL << origin; 
        Bitboard knightMoves = generateKnightMoves(knight, color);

        while(knightMoves != 0) {
            Square destination = __builtin_ctzll(knightMoves);
            moves.push_back(Move(origin, destination, PieceType::KNIGHT, color, MoveType::QUIET));
            //delete move from bitboard
            knightMoves = ~(1ULL << destination) & knightMoves;
        }
        //remove  from knights
        knights = (~knight) & knights;
    }

    //generate rook moves
    Bitboard rooks = this->board.getRooks(color);
    while(rooks != 0) {
        Square origin = __builtin_ctzll(rooks);
        Bitboard rook = 1ULL << origin; 
        Bitboard rookMoves = generateRookMoves(rook, color);

        while(rookMoves != 0) {
            Square destination = __builtin_ctzll(rookMoves);
            moves.push_back(Move(origin, destination, PieceType::ROOK, color, MoveType::QUIET));
            //delete move from bitboard
            rookMoves = ~(1ULL << destination) & rookMoves;
        }
        //remove rook from
        rooks = (~rook) & rooks;
    }

    //generate bishop moves
    Bitboard bishops = this->board.getBishops(color);
    while(bishops != 0) {
        Square origin = __builtin_ctzll(bishops);
        Bitboard bishop = 1ULL << origin; 
        Bitboard bishopMoves = generateBishopMoves(bishop, color);

        while(bishopMoves != 0) {
            Square destination = __builtin_ctzll(bishopMoves);
            moves.push_back(Move(origin, destination, PieceType::BISHOP, color, MoveType::QUIET));
            //delete move from bitboard
            bishopMoves = ~(1ULL << destination) & bishopMoves;
        }
        //remove bishop from
        bishops = (~bishop) & bishop;
    }

    //generate queen moves
    Bitboard queens = this->board.getQueens(color);
    while(queens != 0) {
        Square origin = __builtin_ctzll(queens);
        Bitboard queen = 1ULL << origin; 
        Bitboard queenMoves = generateQueenMoves(queen, color);

        while(queenMoves != 0) {
            Square destination = __builtin_ctzll(queenMoves);
            moves.push_back(Move(origin, destination, PieceType::QUEEN, color, MoveType::QUIET));
            //delete move from bitboard
            queenMoves = ~(1ULL << destination) & queenMoves;
        }
        //remove queen from
        queens = (~queen) & queens;
    }

    //generate pawn moves
    Bitboard pawns = this->board.getPawns(color);
    while(pawns != 0) {
        Square origin = __builtin_ctzll(pawns);
        Bitboard pawn = 1ULL << origin; 
        Bitboard pawnMoves = generatePawnMoves(pawn, color);

        while(pawnMoves != 0) {
            Square destination = __builtin_ctzll(pawnMoves);
            moves.push_back(Move(origin, destination, PieceType::PAWN, color, MoveType::PAWN_PUSH));
            //delete move from bitboard
            pawnMoves = ~(1ULL << destination) & pawnMoves;
        }
        //remove pawn from bitboard
        pawns = (~pawn) & pawns;
    }
        
    return moves;
}

//returns all squares that are north to the current square
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
    if(bishops == (CURRENT_POSITION)) bishops = this->board.getBishops(color); 

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
            //LEGAL MOVE GENERATION: Remove King 
            //Bitboard northWestBlockers = (northWestAttacks & this->board.getOccupiedWithoutWhiteKing());
            Bitboard northWestBlockers = (color == WHITE) ? (northWestAttacks & this->board.getOccupiedWithoutBlackKing())
                : (northWestAttacks & this->board.getOccupiedWithoutWhiteKing());

            if(northWestBlockers != 0ULL) {
                //ctzll returns the number of trailing zeros
                int northWestFirstBlockerIndex = __builtin_ctzll(northWestBlockers);
                Bitboard blockedMoves = MoveGeneration::Northwest(northWestFirstBlockerIndex);
                //if the first blocker is of the same color
                if((1ULL << northWestFirstBlockerIndex) == ((1ULL << northWestFirstBlockerIndex) & (color == BLACK ? this->board.getOccupiedByBlack() : this->board.getOccupiedByWhite()))) {
                    //add first blocker as a blocked moves (different color -> capture)
                    blockedMoves = (blockedMoves | (1ULL << northWestFirstBlockerIndex));
                }
                finalNorthWestMoves = (northWestAttacks ^ blockedMoves);
            } else {
                finalNorthWestMoves = northWestAttacks;
            }
        }

        Bitboard northEastAttacks = MoveGeneration::Northeast(index);
        if(northEastAttacks!= 0ULL) {
            //Bitboard northEastBlockers = (northEastAttacks & this->board.getOccupied());
            //LEGAL MOVE GENERATION: Remove King 
            Bitboard northEastBlockers = (color == WHITE) ? (northEastAttacks & this->board.getOccupiedWithoutBlackKing())
                : (northEastAttacks & this->board.getOccupiedWithoutWhiteKing());

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
            //Bitboard southWestBlockers = (southWestAttacks & this->board.getOccupied());
            //LEGAL MOVE GENERATION: Remove King 
            Bitboard southWestBlockers = (color == WHITE) ? (southWestAttacks & this->board.getOccupiedWithoutBlackKing())
                : (southWestAttacks & this->board.getOccupiedWithoutWhiteKing());

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
            //Bitboard southEastBlockers = (southEastAttacks & this->board.getOccupied());
            //LEGAL MOVE GENERATION: Remove King 
            Bitboard southEastBlockers = (color == WHITE) ? (southEastAttacks & this->board.getOccupiedWithoutBlackKing())
                : (southEastAttacks & this->board.getOccupiedWithoutWhiteKing());

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

        allSinlgeMoves = finalNorthWestMoves | finalNorthEastMoves | finalSouthWestMoves | finalSouthEastMoves;
        totalMoves = (totalMoves | allSinlgeMoves);
        //erase bishop
        bishops = ((1ULL << index) ^ bishops);
    }
    return totalMoves;
}

Bitboard MoveGeneration::generateRookMoves(Bitboard rooks, Color color) {
    if(rooks == (CURRENT_POSITION)) rooks = this->board.getRooks(color); 

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
            //Bitboard northBlockers = (northAttacks & this->board.getOccupied());
            //LEGAL MOVE GENERATION: Remove King 
            Bitboard northBlockers = (color == WHITE) ? (northAttacks & this->board.getOccupiedWithoutBlackKing())
                : (northAttacks & this->board.getOccupiedWithoutWhiteKing());

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
            //Bitboard southBlockers = (southAttacks & this->board.getOccupied());
            //LEGAL MOVE GENERATION: Remove King 
            Bitboard southBlockers = (color == WHITE) ? (southAttacks & this->board.getOccupiedWithoutBlackKing())
                : (southAttacks & this->board.getOccupiedWithoutWhiteKing());

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
            //Bitboard westBlockers = (westAttacks & this->board.getOccupied());
            //LEGAL MOVE GENERATION: Remove King 
            Bitboard westBlockers = (color == WHITE) ? (westAttacks & this->board.getOccupiedWithoutBlackKing())
                : (westAttacks & this->board.getOccupiedWithoutWhiteKing());

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
            //Bitboard eastBlockers = (eastAttacks & this->board.getOccupied());
            //LEGAL MOVE GENERATION: Remove King 
            Bitboard eastBlockers = (color == WHITE) ? (eastAttacks & this->board.getOccupiedWithoutBlackKing())
                : (eastAttacks & this->board.getOccupiedWithoutWhiteKing());
            
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
    if(queens == (CURRENT_POSITION)) queens = this->board.getQueens(color); 
    return (generateBishopMoves(queens, color) | generateRookMoves(queens, color));
}

Bitboard MoveGeneration::generatePawnAttacks(Bitboard pawns, Color color) {
    if(pawns == (CURRENT_POSITION)) pawns = this->board.getPawns(color); 
    if(color == WHITE) {
        //SouthWest
        Bitboard swAttacks = ((pawns & ~FILE_A) << NORTH_WEST); 
        //SouthEast
        Bitboard seAttacks = ((pawns & ~ FILE_H) << NORTH_EAST);

        Bitboard attacks = swAttacks | seAttacks;
        
        return attacks;
    } else {
        //SouthWest
        Bitboard swAttacks = ((pawns & ~FILE_A) >> SOUTH_WEST);
        //SouthEast
        Bitboard seAttacks = ((pawns & ~ FILE_H) >> SOUTH_EAST);

        Bitboard attacks = swAttacks | seAttacks;

        return attacks;
    }
}

Bitboard MoveGeneration::generateEnPassentMoves(Bitboard pawns, Color color) {
    if(pawns == (CURRENT_POSITION)) pawns = this->board.getPawns(color) ;
    if(color == WHITE) {
        //en passent
        bool enPassentPossible = false;
        Bitboard enPassentSquare = EMPTY;
        if(this->board.enPassentTargetSquare != -1) {
            enPassentSquare = (1ULL << this->board.enPassentTargetSquare);
            if(((((pawns & ~FILE_A) << NORTH_WEST) | ((pawns & ~ FILE_H) << NORTH_EAST)) & enPassentSquare) == enPassentSquare) {
                enPassentPossible = true;
            }
        }
        if(enPassentPossible) return enPassentSquare;
        return 0ULL;
    } else {
        //en passent
        bool enPassentPossible = false;
        Bitboard enPassentSquare = EMPTY;
        if(this->board.enPassentTargetSquare != -1) {
            enPassentSquare = (1ULL << this->board.enPassentTargetSquare);
            if(((((pawns & (~FILE_A)) >> SOUTH_WEST) | ((pawns & (~FILE_H)) >> SOUTH_EAST)) & enPassentSquare) == enPassentSquare) {
                enPassentPossible = true;
            }
        }
        if(enPassentPossible) return enPassentSquare;
        return 0ULL;
    }
}

Bitboard MoveGeneration::generatePawnMoves(Bitboard pawns, Color color) {
    if(pawns == (CURRENT_POSITION)) pawns = this->board.getPawns(color); 
    if(color == WHITE) {
        //pawn pushes
        pawns = (pawns == CURRENT_POSITION) ? this->board.whitePawns : pawns;
        //single push
        Bitboard pawnSinglePushes = (pawns << NORTH) & (~board.getOccupied());
        //double push
        Bitboard pawnDoublePushes = ((pawns & RANK_2) << 2*NORTH) & (~board.getOccupied());
        Bitboard pushes = (pawnSinglePushes | pawnDoublePushes);

        Bitboard attacks = (generatePawnAttacks(pawns, color) & this->board.getOccupiedByBlack()) | generateEnPassentMoves(pawns, color); 

        return (pushes | attacks);
    } else {
        //pawn pushes
        pawns = (pawns == CURRENT_POSITION) ? this->board.blackPawns : pawns;
        //single push
        Bitboard pawnSinglePushes = (pawns >> SOUTH) & (~board.getOccupied());
        //double push
        Bitboard pawnDoublePushes = ((pawns & RANK_7) >> 2*SOUTH) & (~board.getOccupied());
        Bitboard pushes = (pawnSinglePushes | pawnDoublePushes);

        Bitboard attacks = (generatePawnAttacks(pawns, color) & this->board.getOccupiedByWhite()) | generateEnPassentMoves(pawns, color);

        return (pushes | attacks);
    }
}

Bitboard MoveGeneration::generateKnightAttacks(Bitboard knights, Color color) {
    if(knights == (CURRENT_POSITION)) knights = (color == WHITE) ? this->board.whiteKnights : this->board.blackKnights;

    Bitboard noNoEaAttacks = ((knights & ~(RANK_7 | RANK_8 | FILE_H)) << NORTH_NORTH_EAST);
    Bitboard noEaEaAttacks = ((knights & ~(RANK_8 | FILE_G | FILE_H)) << NORTH_EAST_EAST);
    Bitboard soEaEaAttacks = ((knights & ~(RANK_1 | FILE_G | FILE_H)) >> SOUTH_EAST_EAST);
    Bitboard soSoEaAttacks = ((knights & ~(RANK_1 | RANK_2 | FILE_H)) >> SOUTH_SOUTH_EAST);
    Bitboard soSoWeAttacks = ((knights & ~(RANK_1 | RANK_2 | FILE_A)) >> SOUTH_SOUTH_WEST);
    Bitboard SoWeWeAttacks = ((knights & ~(RANK_1 | FILE_A | FILE_B)) >> SOUTH_WEST_WEST);
    Bitboard noWeWeAttacks = ((knights & ~(RANK_8 | FILE_A | FILE_B)) << NORTH_WEST_WEST);
    Bitboard noNoWeAttacks = ((knights & ~(RANK_7 | RANK_8 | FILE_A)) << NORTH_NORTH_WEST);

    return noNoEaAttacks | noEaEaAttacks | soEaEaAttacks | soSoEaAttacks | soSoWeAttacks | SoWeWeAttacks | noWeWeAttacks | noNoWeAttacks;
}

Bitboard MoveGeneration::generateKnightMoves(Bitboard knights, Color color) {
    if(knights == (CURRENT_POSITION)) knights = this->board.getKnights(color); 

    Bitboard attacks = generateKnightAttacks(knights, color);

    Bitboard moves;
    if(color == BLACK) {
        moves = (attacks & ~this->board.getOccupiedByBlack());
    } else {
        moves = (attacks & ~this->board.getOccupiedByWhite());
    }

    return moves;
}

Bitboard MoveGeneration::generateKingAttacks(Bitboard king, Color color) {
    if(king == (CURRENT_POSITION)) king = this->board.getKing(color); 

    Bitboard northAttack = ((king & ~(RANK_8)) << NORTH);
    Bitboard westAttack = ((king & ~(FILE_A)) << WEST);
    Bitboard southAttack = ((king & ~(RANK_1)) >> SOUTH);
    Bitboard eastAttack = ((king & ~(FILE_H)) << EAST);

    Bitboard northeastAttack = (king & ~RANK_8 & ~FILE_H) << NORTH_EAST; 
    Bitboard northwestAttack = (king & ~RANK_8 & ~FILE_A) << NORTH_WEST;
    Bitboard southeastAttack = (king & ~RANK_1 & ~FILE_H) >> SOUTH_EAST;
    Bitboard southwestAttack = (king & ~RANK_1 & ~FILE_A) >> SOUTH_WEST;

    return northAttack | westAttack | southAttack | eastAttack | northeastAttack | northwestAttack | southeastAttack | southwestAttack;
}

Bitboard MoveGeneration::generateKingMoves(Bitboard king, Color color) {
    if(king == (CURRENT_POSITION)) king = this->board.getKing(color); 

    Bitboard attacks = generateKingAttacks(king, color);
    std::cout << "king attacks: " << attacks << std::endl;

    //castling
    Bitboard castle = EMPTY;

    if(isInCheck(color).numberOfChecks == 0) {
        //king castle ability
        if(isUnderAttack(this->board.getKing(color) << 1, color).numberOfAttacks == 0
        && isUnderAttack(this->board.getKing(color) << 2, color).numberOfAttacks == 0) {
            if(this->board.getKingSideCastleAbility(color)) {
               castle = castle | (this->board.getKing(color) << 2); 
            }        
        }

        if(isUnderAttack(this->board.getKing(color) >> 1, color).numberOfAttacks == 0
        && isUnderAttack(this->board.getKing(color) >> 2, color).numberOfAttacks == 0) {
            if(this->board.getQueenSideCastleAbility(color)) {
               castle = castle | (this->board.getKing(color) >> 2); 
            }        
        }
    }

    Bitboard pseudoLegalMoves = attacks | castle;

    Bitboard legalMoves;
    if(color == BLACK) {
        legalMoves = (pseudoLegalMoves & ~this->board.getOccupiedByBlack()) & (~generateAttackedSquaresWithoutKing(WHITE)) & (~generateKingAttacks(CURRENT_POSITION, WHITE));
    } else {
        legalMoves = (pseudoLegalMoves & ~this->board.getOccupiedByWhite()) & (~generateAttackedSquaresWithoutKing(BLACK)) & (~generateKingAttacks(CURRENT_POSITION, BLACK));
    }

    return legalMoves;
}

Attack_Info MoveGeneration::isUnderAttack(Square square, Color color) {
    Bitboard squareAsBitboard = 1ULL << square;
    return isUnderAttack(squareAsBitboard, color);
}

Attack_Info MoveGeneration::isUnderAttack(Bitboard squareAsBitboard, Color color) {
    int numberOfAttacks = 0;
    Attack_Info attack_info; 

    //add bishop and diagonal queen moves
    Bitboard moves = generateBishopMoves(squareAsBitboard, color);
    Bitboard intersect = moves & this->board.getBishops(getOppositeColor(color));
    convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::BISHOP, color, MoveType::CAPTURE, numberOfAttacks, attack_info);

    intersect = moves & this->board.getQueens(getOppositeColor(color));
    convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::QUEEN, color, MoveType::CAPTURE, numberOfAttacks, attack_info);

    //add rook and horizontal/vertical queen moves
    moves = generateRookMoves(squareAsBitboard, color);
    intersect = moves & this->board.getRooks(getOppositeColor(color));
    convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::ROOK, color, MoveType::CAPTURE, numberOfAttacks, attack_info);

    intersect = moves & this->board.getQueens(getOppositeColor(color));
    convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::QUEEN, color, MoveType::CAPTURE, numberOfAttacks, attack_info);

    //add knight moves
    moves = generateKnightMoves(squareAsBitboard, color);
    intersect = moves & this->board.getKnights(getOppositeColor(color));
    convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::KNIGHT, color, MoveType::CAPTURE, numberOfAttacks, attack_info);

    //add pawn moves
    moves = generatePawnAttacks(squareAsBitboard, color);
    intersect = moves & this->board.getPawns(getOppositeColor(color));
    convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::PAWN, color, MoveType::CAPTURE, numberOfAttacks, attack_info);

    //add en passent moves
    Bitboard pawns = this->board.getPawns(getOppositeColor(color));
    while(pawns != 0){
        Square pawn = __builtin_ctzll(pawns);
        Bitboard moves = generateEnPassentMoves(squareToBitboard(pawn), getOppositeColor(color));
        if(color == WHITE && moves != 0){
            //en passent square is moved up one row and compared with attacked square
            if((moves << Direction::NORTH) == squareAsBitboard){
                //piece can be captured by en passent
                Move move(pawn, (Square)__builtin_ctzll(moves), PieceType::PAWN, color, MoveType::EN_PASSENT_CAPTURE);
                numberOfAttacks++;
                attack_info.moves.push_back(move);
            }
        } else if(color == BLACK && moves != 0){
            //en passent square is moved down one row and compared with attacked square
            if((moves >> Direction::SOUTH) == squareAsBitboard) {
                Move move(pawn, (Square)__builtin_ctzll(moves), PieceType::PAWN, color, MoveType::EN_PASSENT_CAPTURE);
                numberOfAttacks++;
                attack_info.moves.push_back(move);
            }
        }
        pawns = (~squareToBitboard(pawn)) & pawns;
    }
    attack_info.numberOfAttacks = numberOfAttacks;
    return attack_info;
}

Check_Info MoveGeneration::isInCheck(Color color) {
    return Check_Info(isUnderAttack(__builtin_ctzll(this->board.getKing(color)), color));
}


Bitboard MoveGeneration::generateAttackedSquares(Color color) {
    return generateRookMoves(CURRENT_POSITION, color) | generateBishopMoves(CURRENT_POSITION, color) | generateQueenMoves(CURRENT_POSITION, color) | generateKnightMoves(CURRENT_POSITION, color) | generateKingMoves(CURRENT_POSITION, color) | generatePawnAttacks(CURRENT_POSITION, color);
}

Bitboard MoveGeneration::generateAttackedSquaresWithoutKing(Color color) {
    return generateRookMoves(CURRENT_POSITION, color) | generateBishopMoves(CURRENT_POSITION, color) | generateQueenMoves(CURRENT_POSITION, color) | generateKnightMoves(CURRENT_POSITION, color) | generatePawnAttacks(CURRENT_POSITION, color);
}