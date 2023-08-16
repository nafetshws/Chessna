#include <vector>
#include "../include/moveGeneration.hpp"
#include "../include/board.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

MoveGeneration::MoveGeneration(Board board) {
    this->board = board;
}

u64 MoveGeneration::runPerft(int depth, int maxDepth, Color color) {
    u64 totalNodes = 0ULL;
    u64 nodes = 0ULL;


    if(depth == 0) return 1ULL; 
    
    std::vector<Move> moves = generateMoves(color);
    for(int i = 0; i < moves.size(); i++) {
        Board copyBoard = this->board;
        makeMove(moves.at(i));
        nodes = runPerft(depth-1, maxDepth, getOppositeColor(color));
        totalNodes += nodes;
        if(depth == maxDepth) std::cout << printableMove(moves.at(i)) << ": " << nodes << std::endl;
        //unmake move
        this->board = copyBoard;
    }
    return totalNodes;
}

u64 MoveGeneration::perft(int depth, Color color) {
    return runPerft(depth, depth, color);
}

void MoveGeneration::makeMove(Move move) {
    Bitboard *pieces;

    switch(move.pieceType) {
        case PAWN:
            if(move.color == WHITE) pieces = &this->board.whitePawns;
            else pieces = &this->board.blackPawns; 
            break;
        case ROOK:
            if(move.color == WHITE) pieces = &this->board.whiteRooks;
            else pieces = &this->board.blackRooks; 
            break;
        case KNIGHT:
            if(move.color == WHITE) pieces = &this->board.whiteKnights;
            else pieces = &this->board.blackKnights; 
            break;
        case BISHOP:
            if(move.color == WHITE) pieces = &this->board.whiteBishops;
            else pieces = &this->board.blackBishops; 
            break;
        case QUEEN:
            if(move.color == WHITE) pieces = &this->board.whiteQueen;
            else pieces = &this->board.blackQueen; 
            break;
        case KING:
            if(move.color == WHITE) pieces = &this->board.whiteKing;
            else pieces = &this->board.blackKing; 
            break;
        default:
            break;
    }

    if(move.moveType == QUIET || move.moveType == PAWN_PUSH) {
        *pieces = *pieces & (~squareToBitboard(move.origin)) | squareToBitboard(move.destination);
    } else if(move.moveType == CAPTURE) {
        Piece target = findPiece(move.destination);
        *pieces = *pieces & (~squareToBitboard(move.origin)) | squareToBitboard(move.destination);
        switch(target.type) {
            case PAWN:
                if(target.color == WHITE) this->board.whitePawns ^= squareToBitboard(move.destination);
                else this->board.blackPawns ^= squareToBitboard(move.destination);
                break;
            case ROOK:
                if(target.color == WHITE) this->board.whiteRooks ^= squareToBitboard(move.destination);
                else this->board.blackRooks ^= squareToBitboard(move.destination);
                break;
            case KNIGHT:
                if(target.color == WHITE) this->board.whiteKnights ^= squareToBitboard(move.destination);
                else this->board.whiteKnights ^= squareToBitboard(move.destination);
                break;
            case BISHOP:
                if(target.color == WHITE) this->board.whiteBishops ^= squareToBitboard(move.destination);
                else this->board.blackBishops ^= squareToBitboard(move.destination);
                break;
            case QUEEN:
                if(target.color == WHITE) this->board.whiteQueen ^= squareToBitboard(move.destination);
                else this->board.blackQueen ^= squareToBitboard(move.destination);
                break;
            default:
                break;
        }
    } else if (move.moveType == EN_PASSENT_CAPTURE) {
        *pieces = *pieces & (~squareToBitboard(move.origin)) | squareToBitboard(move.destination);
        if(move.color == WHITE) {
            Bitboard enPassentSquare = squareToBitboard(this->board.enPassentTargetSquare);
            Bitboard target = enPassentSquare >> SOUTH;
            this->board.blackPawns ^= target;
        } else {
            Bitboard enPassentSquare = squareToBitboard(this->board.enPassentTargetSquare);
            Bitboard target = enPassentSquare << NORTH;
            this->board.blackPawns ^= target;
        }
    }


}

std::vector<Move> MoveGeneration::generateMoves(Color color) {
    std::vector<Move> moves;
    //TODO
    //move types are not accurate!!

    //generate all king moves
    Bitboard kingMoves = generateKingMoves(CURRENT_POSITION, color);
    Square kingOrigin = __builtin_ctzll(this->board.getKing(color));

    //transform moves from bitboard into a vector
    while(kingMoves != 0) {
        Square destination = __builtin_ctzll(kingMoves);
        MoveType moveType = (squareToBitboard(destination) & this->board.getOccupiedBy(getOppositeColor(color))) != 0 ? MoveType::CAPTURE : MoveType::QUIET;
        moves.push_back(Move(kingOrigin, destination, PieceType::KING, color, moveType));
        //delete move from bitboard
        kingMoves = ~(1ULL << destination) & kingMoves;
    }

    /*
    * Generate moves when <<color>> is in check
    */
    Check_Info check_info = isInCheck(color);

    //if the king is in double check he has to move
    if(check_info.numberOfChecks >= 2) return moves;

    //TODO: consider pinned pieces
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
                    //TODO: Check for problems!!!
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

                //check whether pawn pushes can block the attack
                Bitboard pawns = this->board.getPawns(color);
                while(pawns != 0) {
                    Square pawn = __builtin_ctzll(pawns);
                    Bitboard pawnMoves = generatePawnMoves(squareToBitboard(pawn), color);
                    if((pawnMoves & squareToBitboard(destination)) != 0) moves.push_back(Move(pawn, destination, PieceType::PAWN, color, MoveType::PAWN_PUSH));
                    pawns = pawns & (~squareToBitboard(pawn));
                }

                intersectionRay = ~(1ULL << destination) & intersectionRay;
            }

        }
        return moves;
    }

    /*
    * Generate moves by pinned pieces and append them to the pinnedPieces Bitboard
    */

    Pins pinnedPieces = getPinnedPieces(color);
    Bitboard pinnedPiecesBitboard = pinnedPieces.absolutePins;

    for(Pin pin : pinnedPieces.pins) {
        Direction direction = pin.direction;
        Square pinnedPieceOriginSquare = __builtin_ctzll(pin.pinnedPiece);

        switch(direction) {
            case Direction::NW: case Direction::SE:
                //only bishops and queens can move along a diagonal pin
                if(pin.pinnedPieceType == BISHOP || pin.pinnedPieceType == QUEEN) {
                    //generate legal moves along pin
                    Bitboard legalMoves = getLegalNorthwestMoves(pin.pinnedPiece, color) | getLegalSoutheastMoves(pin.pinnedPiece, color);
                    //convert bitboard to squares
                    std::vector<Square> destinations = convertBitboardToSquares(legalMoves);

                    //add moves to pins
                    for(Square destination : destinations) {
                        MoveType moveType = (squareToBitboard(destination) & this->board.getOccupiedBy(getOppositeColor(color))) != 0 ? MoveType::CAPTURE : MoveType::QUIET;

                        moves.push_back(Move(pinnedPieceOriginSquare, destination, pin.pinnedPieceType, color, moveType));
                    }
                //if the pawn is pinned it can only capture the pinner
                } else if (pin.pinnedPieceType == PAWN) {
                    if(color == BLACK && direction == Direction::SE) {
                        if(((pin.pinnedPiece >> SOUTH_EAST) & this->board.getOccupiedBy(getOppositeColor(color))) != 0) {
                            moves.push_back(Move(pinnedPieceOriginSquare, bitboardToSquare(pin.pinnedPiece >> SOUTH_EAST), PAWN, color, MoveType::CAPTURE));
                        }
                    } else if(color == WHITE && direction == Direction::NW) {
                        if(((pin.pinnedPiece << NORTH_WEST) & this->board.getOccupiedBy(getOppositeColor(color))) != 0) {
                            moves.push_back(Move(pinnedPieceOriginSquare, bitboardToSquare(pin.pinnedPiece << NORTH_WEST), PAWN, color, MoveType::CAPTURE));
                        }
                    } 
                }
                break;
            case Direction::NE: case Direction::SW:
                //move along pin
                if(pin.pinnedPieceType == BISHOP || pin.pinnedPieceType == QUEEN) {
                    Bitboard legalMoves = getLegalNortheastMoves(pin.pinnedPiece, color) | getLegalSouthwestMoves(pin.pinnedPiece, color);
                    std::vector<Square> destinations = convertBitboardToSquares(legalMoves);

                    for(Square destination : destinations) {
                        MoveType moveType = (squareToBitboard(destination) & this->board.getOccupiedBy(getOppositeColor(color))) != 0 ? MoveType::CAPTURE : MoveType::QUIET;

                        moves.push_back(Move(pinnedPieceOriginSquare, destination, pin.pinnedPieceType, color, moveType));
                    }

                } else if (pin.pinnedPieceType == PAWN) {
                    //caputres only
                    if(color == BLACK && direction == Direction::SW) {
                        if(((pin.pinnedPiece >> SOUTH_WEST) & this->board.getOccupiedBy(getOppositeColor(color))) != 0) {
                            moves.push_back(Move(pinnedPieceOriginSquare, bitboardToSquare(pin.pinnedPiece << SOUTH_WEST), PAWN, color, MoveType::CAPTURE));
                        }
                    } else if(color == WHITE && direction == Direction::NE) {
                        if(((pin.pinnedPiece << NORTH_EAST) & this->board.getOccupiedBy(getOppositeColor(color))) != 0) {
                            moves.push_back(Move(pinnedPieceOriginSquare, bitboardToSquare(pin.pinnedPiece << NORTH_EAST), PAWN, color, MoveType::CAPTURE));
                        }
                    } 
                }
                break;
            case Direction::N: case Direction::S:
                if(pin.pinnedPieceType == ROOK || pin.pinnedPieceType == QUEEN) {
                    Bitboard legalMoves = getLegalNorthMoves(pin.pinnedPiece, color) | getLegalSouthMoves(pin.pinnedPiece, color);
                    std::vector<Square> destinations = convertBitboardToSquares(legalMoves);

                    for(Square destination : destinations) {
                        MoveType moveType = (squareToBitboard(destination) & this->board.getOccupiedBy(getOppositeColor(color))) != 0 ? MoveType::CAPTURE : MoveType::QUIET;

                        moves.push_back(Move(pinnedPieceOriginSquare, destination, pin.pinnedPieceType, color, moveType));
                    }

                } else if (pin.pinnedPieceType == PAWN) {
                    //pawn pushes only
                    if(color == BLACK && direction == Direction::S) {
                        Bitboard pawnSinglePushes = (pin.pinnedPiece >> SOUTH) & (~board.getOccupied());
                        Bitboard pawnDoublePushes = ((pin.pinnedPiece & RANK_7) >> 2*SOUTH) & (~board.getOccupied());
                        Bitboard pawnMoves = pawnSinglePushes | pawnDoublePushes;

                        Square pinnedPieceOriginSquare = (pin.pinnedPiece);
                        std::vector<Square> destinations = convertBitboardToSquares(pawnMoves);

                        for(Square destination : destinations) {
                            MoveType moveType = MoveType::PAWN_PUSH; 
                            moves.push_back(Move(pinnedPieceOriginSquare, destination, pin.pinnedPieceType, color, moveType));
                        }
                    } else if(color == WHITE && direction == Direction::N) {
                        Bitboard pawnSinglePushes = (pin.pinnedPiece << NORTH) & (~board.getOccupied());
                        Bitboard pawnDoublePushes = ((pin.pinnedPiece & RANK_2) << 2*NORTH) & (~board.getOccupied());
                        Bitboard pawnMoves = pawnSinglePushes | pawnDoublePushes;

                        Square pinnedPieceOriginSquare = (pin.pinnedPiece);
                        std::vector<Square> destinations = convertBitboardToSquares(pawnMoves);

                        for(Square destination : destinations) {
                            MoveType moveType = MoveType::PAWN_PUSH; 
                            moves.push_back(Move(pinnedPieceOriginSquare, destination, pin.pinnedPieceType, color, moveType));
                        }
                    } 
                }
                break;
            case Direction::E: case Direction::W:
                if(pin.pinnedPieceType == ROOK || pin.pinnedPieceType == QUEEN) {
                    Bitboard legalMoves = getLegalEastMoves(pin.pinnedPiece, color) | getLegalWestMoves(pin.pinnedPiece, color);
                    std::vector<Square> destinations = convertBitboardToSquares(legalMoves);

                    for(Square destination : destinations) {
                        MoveType moveType = (squareToBitboard(destination) & this->board.getOccupiedBy(getOppositeColor(color))) != 0 ? MoveType::CAPTURE : MoveType::QUIET;

                        moves.push_back(Move(pinnedPieceOriginSquare, destination, pin.pinnedPieceType, color, moveType));
                    }
                } 
                break;
            default:
                break; 
        }
    }
    /*
    * Generate mvoes if the king isn't in check
    */

    //generate pawn moves
    Bitboard pawns = this->board.getPawns(color) &(~pinnedPiecesBitboard);
    while(pawns != 0) {
        Square origin = __builtin_ctzll(pawns);
        Bitboard pawn = 1ULL << origin; 
        Bitboard pawnMoves = generatePawnMoves(pawn, color);

        while(pawnMoves != 0) {
            Square destination = __builtin_ctzll(pawnMoves);
            MoveType moveType = (squareToBitboard(destination) & this->board.getOccupiedBy(getOppositeColor(color))) != 0 ? MoveType::CAPTURE : MoveType::QUIET;
            moves.push_back(Move(origin, destination, PieceType::PAWN, color, moveType));
            //delete move from bitboard
            pawnMoves = ~(1ULL << destination) & pawnMoves;
        }
        //remove pawn from bitboard
        pawns = (~pawn) & pawns;
    }
        
    //generate knight moves
    Bitboard knights = this->board.getKnights(color) & (~pinnedPiecesBitboard);
    while(knights != 0) {
        Square origin = __builtin_ctzll(knights);
        Bitboard knight = 1ULL << origin; 
        Bitboard knightMoves = generateKnightMoves(knight, color);

        while(knightMoves != 0) {
            Square destination = __builtin_ctzll(knightMoves);
            MoveType moveType = (squareToBitboard(destination) & this->board.getOccupiedBy(getOppositeColor(color))) != 0 ? MoveType::CAPTURE : MoveType::QUIET;
            moves.push_back(Move(origin, destination, PieceType::KNIGHT, color, moveType));
            //delete move from bitboard
            knightMoves = ~(1ULL << destination) & knightMoves;
        }
        //remove  from knights
        knights = (~knight) & knights;
    }

    //generate rook moves
    Bitboard rooks = this->board.getRooks(color) & (~pinnedPiecesBitboard);

    while(rooks != 0) {
        Square origin = __builtin_ctzll(rooks);
        Bitboard rook = 1ULL << origin; 
        Bitboard rookMoves = generateRookMoves(rook, color);

        while(rookMoves != 0) {
            Square destination = __builtin_ctzll(rookMoves);
            MoveType moveType = (squareToBitboard(destination) & this->board.getOccupiedBy(getOppositeColor(color))) != 0 ? MoveType::CAPTURE : MoveType::QUIET;
            moves.push_back(Move(origin, destination, PieceType::ROOK, color, moveType));
            //delete move from bitboard
            rookMoves = ~(1ULL << destination) & rookMoves;
        }
        //remove rook from
        rooks = (~rook) & rooks;
    }

    //generate bishop moves
    Bitboard bishops = this->board.getBishops(color) & (~pinnedPiecesBitboard);
    while(bishops != 0) {
        Square origin = __builtin_ctzll(bishops);
        Bitboard bishop = 1ULL << origin; 
        Bitboard bishopMoves = generateBishopMoves(bishop, color);

        while(bishopMoves != 0) {
            Square destination = __builtin_ctzll(bishopMoves);
            MoveType moveType = (squareToBitboard(destination) & this->board.getOccupiedBy(getOppositeColor(color))) != 0 ? MoveType::CAPTURE : MoveType::QUIET;
            moves.push_back(Move(origin, destination, PieceType::BISHOP, color, moveType));
            //delete move from bitboard
            bishopMoves = ~(1ULL << destination) & bishopMoves;
        }
        //remove bishop from
        bishops = (~bishop) & bishops;
    }

    //generate queen moves
    Bitboard queens = this->board.getQueens(color) &(~pinnedPiecesBitboard);
    while(queens != 0) {
        Square origin = __builtin_ctzll(queens);
        Bitboard queen = 1ULL << origin; 
        Bitboard queenMoves = generateQueenMoves(queen, color);

        while(queenMoves != 0) {
            Square destination = __builtin_ctzll(queenMoves);
            MoveType moveType = (squareToBitboard(destination) & this->board.getOccupiedBy(getOppositeColor(color))) != 0 ? MoveType::CAPTURE : MoveType::QUIET;
            moves.push_back(Move(origin, destination, PieceType::QUEEN, color, moveType));
            //delete move from bitboard
            queenMoves = ~(1ULL << destination) & queenMoves;
        }
        //remove queen from
        queens = (~queen) & queens;
    }

    return moves;
}

Bitboard MoveGeneration::generateBishopMoves(Bitboard bishops, Color color) {
    if(bishops == (CURRENT_POSITION)) bishops = this->board.getBishops(color); 

    Bitboard allSinlgeMoves = EMPTY;
    Bitboard totalMoves = EMPTY;

    while(bishops > 0) {
        int index = 63 - __builtin_clzll(bishops);
        Bitboard finalNorthWestMoves = MoveGeneration::getLegalNorthwestMoves(bishops, color);
        Bitboard finalNorthEastMoves = MoveGeneration::getLegalNortheastMoves(bishops, color);
        Bitboard finalSouthWestMoves = MoveGeneration::getLegalSouthwestMoves(bishops, color);
        Bitboard finalSouthEastMoves = MoveGeneration::getLegalSoutheastMoves(bishops, color);


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
        Bitboard finalNorthMoves = MoveGeneration::getLegalNorthMoves(rooks, color);
        Bitboard finalSouthMoves = MoveGeneration::getLegalSouthMoves(rooks, color);
        Bitboard finalWestMoves  = MoveGeneration::getLegalWestMoves(rooks, color);
        Bitboard finalEastMoves  = MoveGeneration::getLegalEastMoves(rooks, color);

        //clzll returns the number of leading zeros
        int index = 63 - __builtin_clzll(rooks);

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

bool MoveGeneration::checkForEnPassenDiscoveredCheck(Bitboard targetPawn, Bitboard attackerPawn, Color color) {
    if(color == WHITE) {
        this->board.whitePawns = this->board.whitePawns & (~attackerPawn);
        this->board.blackPawns = this->board.blackPawns & (~targetPawn);
    } else {
        this->board.blackPawns = this->board.blackPawns & (~attackerPawn);
        this->board.whitePawns = this->board.whitePawns & (~targetPawn);
    }

    Color oppositeColor = getOppositeColor(color);
    bool enPassentPossible = false;

    Bitboard kingMovesEast = getLegalEastMoves(this->board.getKing(color), color);
    Bitboard kingMovesWest = getLegalWestMoves(this->board.getKing(color), color);
    Bitboard rookMovesEast = getAllLegalMovesOf(this->board.getRooks(oppositeColor), Direction::E, oppositeColor);
    Bitboard queenMovesEast = getAllLegalMovesOf(this->board.getQueens(oppositeColor), Direction::E, oppositeColor);
    Bitboard rookMovesWest = getAllLegalMovesOf(this->board.getRooks(oppositeColor), Direction::W, oppositeColor);
    Bitboard queenMovesWest = getAllLegalMovesOf(this->board.getQueens(oppositeColor), Direction::W, oppositeColor);

    if(((kingMovesEast & rookMovesWest) != 0) || (kingMovesEast & queenMovesWest) != 0
        || (kingMovesWest & rookMovesEast) != 0 || (kingMovesWest & queenMovesEast) != 0) {
            enPassentPossible = false;
    } else {
        enPassentPossible = true;
    }

    if(color == WHITE) {
        this->board.whitePawns = this->board.whitePawns | attackerPawn; 
        this->board.blackPawns = this->board.blackPawns | targetPawn;
    } else {
        this->board.blackPawns = this->board.blackPawns | attackerPawn;
        this->board.whitePawns = this->board.whitePawns | targetPawn;
    }
    return enPassentPossible;
}

Bitboard MoveGeneration::generateEnPassentMoves(Bitboard pawns, Color color) {
    if(pawns == (CURRENT_POSITION)) pawns = this->board.getPawns(color) ;

    Color oppositeColor = getOppositeColor(color);

    if(color == WHITE) {
        //en passent
        bool enPassentPossible = false;
        Bitboard enPassentSquare = EMPTY;
        if(this->board.enPassentTargetSquare != -1) {
            enPassentSquare = (1ULL << this->board.enPassentTargetSquare);
            if((((pawns & ~FILE_A) << NORTH_WEST) & enPassentSquare) == enPassentSquare) {
                Bitboard targetPawn = enPassentSquare >> SOUTH;
                Bitboard attackerPawn = enPassentSquare >> SOUTH_EAST;
                enPassentPossible = checkForEnPassenDiscoveredCheck(targetPawn, attackerPawn, color);
            } else if((((pawns & (~FILE_H)) >> NORTH_EAST) & enPassentSquare) == enPassentSquare) {
                Bitboard targetPawn = enPassentSquare << SOUTH;
                Bitboard attackerPawn = enPassentSquare << SOUTH_WEST;
                enPassentPossible = checkForEnPassenDiscoveredCheck(targetPawn, attackerPawn, color);
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
            if((((pawns & (~FILE_A)) >> SOUTH_WEST) & enPassentSquare) == enPassentSquare) {
                Bitboard targetPawn = enPassentSquare << NORTH;
                Bitboard attackerPawn = enPassentSquare << NORTH_EAST;
                enPassentPossible = checkForEnPassenDiscoveredCheck(targetPawn, attackerPawn, color);
            } else if((((pawns & (~FILE_H)) >> SOUTH_EAST) & enPassentSquare) == enPassentSquare) {
                Bitboard targetPawn = enPassentSquare << NORTH;
                Bitboard attackerPawn = enPassentSquare << NORTH_WEST;
                enPassentPossible = checkForEnPassenDiscoveredCheck(targetPawn, attackerPawn, color);
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
        Bitboard pawnDoublePushes = ((pawns & RANK_2) << NORTH) & (~board.getOccupied());
        pawnDoublePushes = (pawnDoublePushes << NORTH) & (~board.getOccupied());
        Bitboard pushes = (pawnSinglePushes | pawnDoublePushes);

        Bitboard attacks = (generatePawnAttacks(pawns, color) & this->board.getOccupiedByBlack()) | generateEnPassentMoves(pawns, color); 

        return (pushes | attacks);
    } else {
        //pawn pushes
        pawns = (pawns == CURRENT_POSITION) ? this->board.blackPawns : pawns;
        //single push
        Bitboard pawnSinglePushes = (pawns >> SOUTH) & (~board.getOccupied());
        //double push
        Bitboard pawnDoublePushes = ((pawns & RANK_7) >> SOUTH) & (~board.getOccupied());
        pawnDoublePushes = (pawnDoublePushes >> SOUTH) & (~board.getOccupied());
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
    Bitboard westAttack = ((king & ~(FILE_A)) >> WEST);
    Bitboard southAttack = ((king & ~(RANK_1)) >> SOUTH);
    Bitboard eastAttack = ((king & ~(FILE_H)) << EAST);

    Bitboard directAttacks = westAttack; 

    Bitboard northeastAttack = (king & ~RANK_8 & ~FILE_H) << NORTH_EAST; 
    Bitboard northwestAttack = (king & ~RANK_8 & ~FILE_A) << NORTH_WEST;
    Bitboard southeastAttack = (king & ~RANK_1 & ~FILE_H) >> SOUTH_EAST;
    Bitboard southwestAttack = (king & ~RANK_1 & ~FILE_A) >> SOUTH_WEST;

    return northAttack | westAttack | southAttack | eastAttack | northeastAttack | northwestAttack | southeastAttack | southwestAttack;
}

Bitboard MoveGeneration::generateKingMoves(Bitboard king, Color color) {
    if(king == (CURRENT_POSITION)) king = this->board.getKing(color); 

    Bitboard attacks = generateKingAttacks(king, color);

    //castling
    Bitboard castle = EMPTY;

    if(isInCheck(color).numberOfChecks == 0) {
        //king castle ability
        if(isUnderAttack(this->board.getKing(color) << 1, color).numberOfAttacks == 0
        && isUnderAttack(this->board.getKing(color) << 2, color).numberOfAttacks == 0
        && ((this->board.getKing(color) << 1) & this->board.getOccupied()) == 0
        && ((this->board.getKing(color) << 2) & this->board.getOccupied()) == 0) {
            if(this->board.getKingSideCastleAbility(color)) {
               castle = castle | (this->board.getKing(color) << 2); 
            }        
        }

        if(isUnderAttack(this->board.getKing(color) >> 1, color).numberOfAttacks == 0
        && isUnderAttack(this->board.getKing(color) >> 2, color).numberOfAttacks == 0
        && ((this->board.getKing(color) >> 1) & this->board.getOccupied()) == 0
        && ((this->board.getKing(color) >> 2) & this->board.getOccupied()) == 0) {
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

    Pins pins = getPinnedPieces(getOppositeColor(color));

    //std::cout << "absolute pins: " << pins.absolutePins << std::endl;

    //add bishop and diagonal queen moves
    Bitboard moves = generateBishopMoves(squareAsBitboard, color);
    Bitboard intersect = moves & (this->board.getBishops(getOppositeColor(color)) & (~pins.absolutePins));
    convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::BISHOP, color, MoveType::CAPTURE, numberOfAttacks, attack_info);

    intersect = moves & (this->board.getQueens(getOppositeColor(color)) & (~pins.absolutePins));
    convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::QUEEN, color, MoveType::CAPTURE, numberOfAttacks, attack_info);

    //add rook and horizontal/vertical queen moves
    moves = generateRookMoves(squareAsBitboard, color);
    intersect = moves & (this->board.getRooks(getOppositeColor(color)) & (~pins.absolutePins));
    convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::ROOK, color, MoveType::CAPTURE, numberOfAttacks, attack_info);

    intersect = moves & (this->board.getQueens(getOppositeColor(color)) & (~pins.absolutePins));
    convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::QUEEN, color, MoveType::CAPTURE, numberOfAttacks, attack_info);

    //add knight moves
    moves = generateKnightMoves(squareAsBitboard, color);
    intersect = moves & (this->board.getKnights(getOppositeColor(color)) & (~pins.absolutePins));
    convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::KNIGHT, color, MoveType::CAPTURE, numberOfAttacks, attack_info);

    //add pawn moves
    moves = generatePawnAttacks(squareAsBitboard, color); //& this->board.getOccupiedBy(getOppositeColor(color));
    if((squareAsBitboard & this->board.getOccupiedBy(color)) != 0) {
        intersect = moves & (this->board.getPawns(getOppositeColor(color)) & (~pins.absolutePins));
        convertBitbaordToMoves(intersect, squareAsBitboard, PieceType::PAWN, color, MoveType::CAPTURE, numberOfAttacks, attack_info);
    }

    //add en passent moves
    Bitboard pawns = this->board.getPawns(getOppositeColor(color)) & (~pins.absolutePins);
    while(pawns != 0){
        Square pawn = __builtin_ctzll(pawns);
        Bitboard moves = generateEnPassentMoves(squareToBitboard(pawn), getOppositeColor(color));
        if(color == WHITE && moves != 0){
            //en passent square is moved up one row and compared with attacked square
            if((moves << DirectionValues::NORTH) == squareAsBitboard){
                //piece can be captured by en passent
                Move move(pawn, (Square)__builtin_ctzll(moves), PieceType::PAWN, color, MoveType::EN_PASSENT_CAPTURE);
                numberOfAttacks++;
                attack_info.moves.push_back(move);
            }
        } else if(color == BLACK && moves != 0){
            //en passent square is moved down one row and compared with attacked square
            if((moves >> DirectionValues::SOUTH) == squareAsBitboard) {
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

Piece MoveGeneration::findPiece(Square s) {
    Bitboard position = squareToBitboard(s);
    Color color;
    PieceType pieceType = UNOCCUPIED;
    
    if((position & this->board.getOccupiedBy(WHITE)) != 0) color = WHITE;
    else color = BLACK;

    if((this->board.getPawns(color) & position) != 0) pieceType = PAWN;
    if((this->board.getRooks(color) & position) != 0) pieceType = ROOK;
    if((this->board.getKnights(color) & position) != 0) pieceType = KNIGHT;
    if((this->board.getBishops(color) & position) != 0) pieceType = BISHOP;
    if((this->board.getQueens(color) & position) != 0) pieceType = QUEEN;
    if((this->board.getKing(color) & position) != 0) pieceType = KING;

    return Piece(s, pieceType, color);
}

Bitboard MoveGeneration::checkForPinnedPieces(Bitboard kingMoves, Bitboard pieceMoves, Bitboard piece2Moves, PieceType pieceType, PieceType piece2Type, Direction direction, Color color, Pins &pins) {
    //to find a pinned piece you can check whether the intersection of king moves and moves of the opponent pieces intersect on the position of a piece
    Bitboard intersection = (kingMoves & pieceMoves) & this->board.getOccupiedBy(color);
    if(intersection != 0) pins.pins.push_back(Pin(this->board.getPieceTypeOfSquare(intersection, color), pieceType, intersection, direction));
    //the same thing for the queen
    Bitboard intersection2 = (kingMoves & piece2Moves) & this->board.getOccupiedBy(color);
    if(intersection2 != 0) pins.pins.push_back(Pin(this->board.getPieceTypeOfSquare(intersection2, color), piece2Type, intersection2, direction));

    return intersection | intersection2;
}


Pins MoveGeneration::getPinnedPieces(Color color){
    Bitboard pinnedPiecesBitboard = EMPTY;
    Color oppositeColor = getOppositeColor(color);

    Pins pins;
    Bitboard intersection = EMPTY;
    Bitboard intersection2 = EMPTY;

    Bitboard king = this->board.getKing(color);
    Bitboard oppositeBishops = this->board.getBishops(oppositeColor);
    Bitboard oppositeRooks = this->board.getRooks(oppositeColor);
    Bitboard oppositeQueens = this->board.getQueens(oppositeColor);

    std::vector<Direction> dirs = {SW, SE, NW, NE};
    std::vector<Direction> oppositeDirs = {NE, NW, SE, SW};

    //check for pinned pieces on diagonal lines
    for(int i = 0; i < dirs.size(); i++) {
        Bitboard kingMoves = getAllLegalMovesOf(king, dirs.at(i), oppositeColor);
        Bitboard bishopMoves = getAllLegalMovesOf(oppositeBishops, oppositeDirs.at(i), oppositeColor);
        Bitboard queenMoves = getAllLegalMovesOf(oppositeQueens, oppositeDirs.at(i), oppositeColor);

        pinnedPiecesBitboard = pinnedPiecesBitboard | checkForPinnedPieces(kingMoves, bishopMoves, queenMoves, BISHOP, QUEEN, dirs.at(i), color, pins);
    }

     dirs = {S, N, E, W};
     oppositeDirs = {N, S, W, E};

    //check for pinned pieces on straight lines
    for(int i = 0; i < dirs.size(); i++) {
        Bitboard kingMoves = getAllLegalMovesOf(king, dirs.at(i), oppositeColor);
        Bitboard rooksMoves = getAllLegalMovesOf(oppositeRooks, oppositeDirs.at(i), oppositeColor);
        Bitboard queenMoves = getAllLegalMovesOf(oppositeQueens, oppositeDirs.at(i), oppositeColor);

        pinnedPiecesBitboard = pinnedPiecesBitboard | checkForPinnedPieces(kingMoves, rooksMoves, queenMoves, ROOK, QUEEN, dirs.at(i), color, pins);
    }

    pins.absolutePins = pinnedPiecesBitboard;

    return pins;
}

//returns all squares that are north to the current square
Bitboard MoveGeneration::North(Square square) {
    int rank = square / 8;
    Bitboard attacks = EMPTY;

    if(rank == 7) return attacks;

    attacks = (1ULL << (square + DirectionValues::NORTH));
    rank++;

    while(rank < 7) {
        attacks = attacks | (attacks << DirectionValues::NORTH); 
        rank++;
    }
    return attacks;
}

Bitboard MoveGeneration::Northeast(Square square) {
    int rank = square / 8;
    int file = square % 8;
    Bitboard attacks = EMPTY;

    if(rank == 7 || file == 7) return attacks;

    attacks = (1ULL << (square + DirectionValues::NORTH_EAST));
    rank++;
    file++;

    while(rank < 7 && file < 7) {
        attacks = attacks | (attacks << DirectionValues::NORTH_EAST); 
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

    attacks = (1ULL << (square + DirectionValues::NORTH_WEST));
    rank++;
    file--;

    while(rank < 7 && file > 0) {
        attacks = attacks | (attacks << DirectionValues::NORTH_WEST); 
        rank++;
        file--;
    }
    return attacks;
}


Bitboard MoveGeneration::South(Square square) {
    int rank = square / 8;
    Bitboard attacks = EMPTY;

    if(rank == 0) return attacks;

    attacks = (1ULL << (square - DirectionValues::SOUTH));
    rank--;

    while(rank > 0) {
        attacks = attacks | (attacks >> DirectionValues::SOUTH); 
        rank--;
    }
    return attacks;
}

Bitboard MoveGeneration::Southeast(Square square) {
    int rank = square / 8;
    int file = square % 8;
    Bitboard attacks = EMPTY;

    if(rank == 0 || file == 7) return attacks;

    attacks = (1ULL << (square - DirectionValues::SOUTH_EAST));
    rank--;
    file++;

    while(rank > 0 && file < 7) {
        attacks = attacks | (attacks >> DirectionValues::SOUTH_EAST); 
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

    attacks = (1ULL << (square - DirectionValues::SOUTH_WEST));
    rank--;
    file--;

    while(rank > 0 && file > 0) {
        attacks = attacks | (attacks >> DirectionValues::SOUTH_WEST); 
        rank--;
        file--;
    }
    return attacks;
}

Bitboard MoveGeneration::West(Square square) {
    int file = square % 8;
    Bitboard attacks = EMPTY; 

    if(file == 0) return attacks;

    attacks = (1ULL << (square - DirectionValues::WEST));
    file--;

    while(file > 0) {
        attacks = attacks | (attacks >> DirectionValues::WEST);
        file--;
    }

    return attacks;
}

Bitboard MoveGeneration::East(Square square) {
    int file = square % 8;
    Bitboard attacks = EMPTY; 

    if(file == 7) return attacks;

    attacks = (1ULL << (square + DirectionValues::EAST));
    file++;

    while(file < 7) {
        attacks = attacks | (attacks << DirectionValues::EAST);
        file++;
    }
    return attacks;
}


Bitboard MoveGeneration::getLegalNorthwestMoves(Bitboard pieces, Color color){
    //clzll returns the number of leading zeros
    int index = 63 - __builtin_clzll(pieces);
    Bitboard northWestAttacks = MoveGeneration::Northwest(index);
    if(northWestAttacks != EMPTY) {
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
            return (northWestAttacks ^ blockedMoves);
        } else {
            return northWestAttacks;
        }
    }
    return EMPTY;
}

Bitboard MoveGeneration::getLegalNortheastMoves(Bitboard pieces, Color color){
    int index = 63 - __builtin_clzll(pieces);
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
            return (northEastAttacks ^ blockedMoves);
        } else {
            return northEastAttacks;
        }
    }
    return EMPTY;
}
Bitboard MoveGeneration::getLegalSouthwestMoves(Bitboard pieces, Color color){
    int index = 63 - __builtin_clzll(pieces);
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
            return (southWestAttacks ^ blockedMoves);
        } else {
            return southWestAttacks;
        }
    }
    return EMPTY;
}
Bitboard MoveGeneration::getLegalSoutheastMoves(Bitboard pieces, Color color){
    int index = 63 - __builtin_clzll(pieces);
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
            return (southEastAttacks ^ blockedMoves);
        } else {
            return southEastAttacks;
        }
    }
    return EMPTY;
}


Bitboard MoveGeneration::getLegalNorthMoves(Bitboard pieces, Color color){
    //clzll returns the number of leading zeros
    int index = 63 - __builtin_clzll(pieces);
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
            return (northAttacks ^ blockedMoves);
        } else {
            return northAttacks;
        }
    }
    return EMPTY;
}
Bitboard MoveGeneration::getLegalSouthMoves(Bitboard pieces, Color color){
    int index = 63 - __builtin_clzll(pieces);
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
            return (southAttacks ^ blockedMoves);
        } else {
            return southAttacks;
        }
    }
    return EMPTY;
}
Bitboard MoveGeneration::getLegalWestMoves(Bitboard pieces, Color color){
    int index = 63 - __builtin_clzll(pieces);
    Bitboard westAttacks = MoveGeneration::West(index);
    //std::cout << "west of square " << index << ": " << westAttacks << std::endl;
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
            return (westAttacks ^ blockedMoves);
        } else {
            return westAttacks;
        }
    }
    return EMPTY;
}
Bitboard MoveGeneration::getLegalEastMoves(Bitboard pieces, Color color){
    int index = 63 - __builtin_clzll(pieces);
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
            return (eastAttacks ^ blockedMoves);
        } else {
            return eastAttacks;
        }
    }
    return EMPTY;
}

Bitboard MoveGeneration::getAllLegalMovesOf(Bitboard pieces, Direction direction, Color color) {
    Bitboard ret = EMPTY;

    while(pieces != 0) {
        Square index = __builtin_ctzll(pieces);
        Bitboard b = squareToBitboard(index);
        switch(direction) {
            case Direction::SW:
                ret = ret | getLegalSouthwestMoves(b, color);
                break;
            case Direction::SE:
                ret = ret | getLegalSoutheastMoves(b, color);
                break;
            case Direction::NW:
                ret = ret | getLegalNorthwestMoves(b, color);
                break;
            case Direction::NE:
                ret = ret | getLegalNortheastMoves(b, color);
                break;
            case Direction::N:
                ret = ret | getLegalNorthMoves(b, color);
                break;
            case Direction::S:
                ret = ret | getLegalSouthMoves(b, color);
                break;
            case Direction::E:
                ret = ret | getLegalEastMoves(b, color);
                break;
            case Direction::W:
                ret = ret | getLegalWestMoves(b, color);
                break;
            default:
                break;
        }
        //remove piece bit from bitboard
        pieces = pieces & (~b);
    }
    return ret;
}